# include "../../dup/dataType.h"
# include "server.h"
# include <stdlib.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include "serverParsing.h"
# include "serverUtiles.h"



static int server_closing(server_data* server) {
  send_str_all(server, "01:exit server closing goodbye!\n", NULL);
  return 0;
}

static int try_bind(t_setting* data, server_data* server) {
  server->servaddr.sin_port = htons(server->port);
  if (bind(server->socketFd,(const struct sockaddr *)&(server->servaddr) , sizeof(server->servaddr))) {
    put_str_error(data, YEL, "can't bind: %d", server->port);
    perror("bind");
    return 1;
  }
  return 0;
}

int init_server(t_setting* data, server_data* server, size_t nbArgs) {
  memset(server, 0, sizeof(*server));
  bzero(&server->servaddr, sizeof(server->servaddr));
  const char* password = fv_get_value(data->flagValue, 0);
  printf("password: %s\n", password);
  memcpy(server->password, password, strlen(password));
  if (nbArgs == 2) {
    const char* port = fv_get_value(data->flagValue, 1);
    printf("port: %s\n", port);
    server->port = atoi(port);
  }
  else {
    server->port = DEF_PORT;
    printf("def port: %d\n", DEF_PORT);
  }
  server->socketFd = socket(AF_INET, SOCK_STREAM, 0);
  server->maxFd = server->socketFd;
  if (server->socketFd < 0) {
    perror("socket");
    return 1;
  }
  //int flags = fcntl(server->socketFd, F_GETFL, 0);
  //flags = flags & ~O_NONBLOCK;
  //fcntl(server->socketFd, F_SETFL, flags);
  server->servaddr.sin_family = AF_INET;
  server->servaddr.sin_addr.s_addr = INADDR_ANY;
  if (try_bind(data, server))
    return 1;
  if (listen(server->socketFd, MAX_USER) < 0) {
    perror("listen");
    return 1;
  }
  server->nbUser = server->nbUser;
  printf("server about to start\n");
  return 0;
}

int setup_server_user(server_data* server) {
  server->ids = 2;
  server->nbUser = server->socketFd;
  server->userData.users = calloc(MAX_USER + 1, sizeof(*server->userData.users));
  if (!server->userData.users) {
    perror("calloc");
    return 1;
  }
  FD_ZERO(&server->as);
  FD_SET(server->socketFd, &server->as);
  server->userData.users[0] = (t_user){
    .id = 1,
    .fd = server->socketFd,
    .status = 0,
    .name = "self",
    .ip = "local",
    .msg = NULL,
    .setting = NULL,
  };
  return 0;
}

static int rm_user_message(server_data* server, size_t i) {
  if (i >= MAX_USER)
    return 1;
  free(server->userData.users[i].msg);
  server->userData.users[i].msg = NULL;
  return 0;
}

int server_clean_up(t_setting* data, server_data* server) {
  for (size_t i = 0; i < MAX_USER; i++) {
    rm_user_message(server, i);
  }
  free(server->userData.users);
  fv_free(&data->flagValue);
  free_block(server->def.block);
  return 0;
}

static int add_user(int fd, server_data* server) {
  // if it works i don't know why
  server->maxFd = fd > server->maxFd ? fd : server->maxFd;
  FD_SET(fd, &server->as);
  server->ids++;
  server->nbUser++;
  const char* addres = inet_ntoa(server->servaddr.sin_addr);
  const size_t addres_len = strlen(addres);
  unsigned int i  = 1;
  printf("add nb:%u\nid:%u\n", server->nbUser, server->ids); //! log
  while (i < MAX_USER) {
    if (server->userData.users[i].id == 0)
      break ;
    i++;
  }
  if (i == MAX_USER) {
    //! need to close fd and other info
    printf("out of space can't add user\n");
    return 1;
  }
  server->userData.users[i] = (t_user) {
    .id = server->ids,
    .fd = fd,
    .status = 0,
    .name = "",
    .ip = "",
    .msg = NULL,
    .setting = NULL
  };
  memcpy(server->userData.users[i].ip, addres, addres_len + 1);
  //server->userData.regist++; // not true 
  printf("ip: %s\nindex: %u\n", addres, i);
  //send_str_all(server, "new user\n", "server:");
  return 0;
}


static int wipe_user(server_data* server) {
  if (!read_byte(server->userData.users[server->userData.read].status, valid)) {
    bzero(&server->userData.users[server->userData.read], sizeof(t_user));
    printf("user %zu was wipe\n", server->userData.read);
  }
  return 0;
}


static int remove_user(server_data* server) {
  server->nbUser--;
  t_user* u = server->userData.users + server->userData.read;
  FD_CLR(u->fd, &server->as);
  close(u->fd);
  //u->fd = 0;
  rm_user_message(server, server->userData.read);
  return 0;
}



static int edit_user_msg(server_data* server, const char* b) {
  t_user* u = server->userData.users + server->userData.read;
  if (u->msg) {
    u->msg = join_free(u->msg, b);
  } else {
    u->msg = d__strdup(b);
  }
  return u->msg ? 0 : 1;
}

static void clean_str(char* s, ssize_t l) {
  if (l > 0) {
    if (s[l - 1] == '\n') {
      s[l - 1] = 0;
    }
  }
}

static int manage_user(server_data* server) {
  char buff[READ_BUFF_SIZE + 1];
  const ssize_t readByte = recv(server->userData.users[server->userData.read].fd, buff , READ_BUFF_SIZE, 0);
  if (readByte <= 0) {
    remove_user(server);
    wipe_user(server);
    printf("dc\n");
    return 0;
  }
  buff[readByte] = 0;
  clean_str(buff, readByte);
  edit_user_msg(server, buff);
  printf("%s:%s\n", server->userData.users[server->userData.read].ip, server->userData.users[server->userData.read].msg);
  return dispatch(server);
}



int network_loop(server_data* server) {
  int kill = 0;
  while (!kill) {
    server->rs = server->ws = server->as;
    const int selectResult = select(server->maxFd + 1, &server->rs, &server->ws, NULL, NULL);
    if (selectResult < 0) {
      perror("select");
      return 1;
    }
    for (server->userData.read = 0; server->userData.read < MAX_USER; server->userData.read++) {
      if (read_byte((server->userData.users + server->userData.read)->status, force_dc)) {
        printf("remove is ass\n");
        remove_user(server);
        wipe_user(server);
      }
      if(!FD_ISSET(server->userData.users[server->userData.read].fd, &server->rs)) {
        continue ;
      }
      if (server->userData.users[server->userData.read].fd == server->socketFd) {
        socklen_t addr_len = sizeof(server->servaddr);
        const int clId = accept(server->socketFd, (struct sockaddr *)&server->servaddr, &addr_len);
        if (clId >= 0) {
          add_user(clId, server);
          break;
        }
        else {
          perror("accept");
        }
      }
      else {
        kill = manage_user(server);
      }
    }
  }
  server_closing(server);
  return 0;
}