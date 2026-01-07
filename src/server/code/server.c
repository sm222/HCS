# include "../../dup/dataType.h"
# include "server.h"
# include <stdlib.h>
# include <arpa/inet.h>
# include "serverParsing.h"



int send_to_user(server_data* server, t_user* u, const char* msg) {
  if (!u || !server) {
    return 2;
  }
  send(u->fd, msg, strlen(msg), 0);
  return 0;
}

int send_str(server_data* server, size_t i, const char* msg) {
  if (i >= MAX_USER)
    return 1;
  t_user* u = server->userData.users + i;
  send_to_user(server, u, msg);
  return 0;
}

static int server_closing(server_data* server) {
  send_str_all(server, "01:exit server closing goodbye!\n", NULL);
  return 0;
}

int send_str_all(server_data* server, const char* msg, const char* from) {
  for (size_t i  = 1; i < MAX_USER; i++) {
    t_user* u = server->userData.users + i;
    if (u->fd) {
      if (from) {
        send_str(server, i, from);
      }
      send_str(server, i, msg);
    }
  }
  return 0;
}

/*
int send_to_channel(server_data* server, channel* chan, t_user* from, const char* msg) {
  if (!server) {
    return 2;
  }
  if (!chan || !from) {
    return 3;
  }
  
}
*/

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
  if (server->socketFd < 0) {
    perror("socket");
    return 1;
  }
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

int setup_server_user(t_setting* data, server_data* server) {
  server->ids = 0;
  server->nbUser = server->socketFd;
  server->userData.users = calloc(MAX_USER + 1, sizeof(*server->userData.users));
  if (!server->userData.users) {
    perror("calloc");
    return 1;
  }
  (void)data; // safety
  FD_ZERO(&server->as);
  FD_SET(server->socketFd, &server->as);
  server->userData.users[0] = (t_user){
    .id = 0,
    .fd = server->socketFd,
    .status = 0,
    .name = "self",
    .ip = "local",
    .msg = NULL,
    .setting = NULL,
  };
  return 0;
}

static int rm_message(server_data* server, size_t i) {
  if (i >= MAX_USER)
    return 1;
  free(server->userData.users[i].msg);
  server->userData.users[i].msg = NULL;
  return 0;
}

int server_clean_up(t_setting* data, server_data* server) {
  for (size_t i = 0; i < MAX_USER; i++) {
    rm_message(server, i);
  }
  free(server->userData.users);
  fv_free(&data->flagValue);
  free_block(server->def.block);
  return 0;
}

static int add_user(int fd, server_data* server) {
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
    // need to close fd and other info
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
  server->userData.regist++;
  printf("ip: %s\nindex: %u\n", addres, i);
  send_str_all(server, "new user\n", "server:");
  return 0;
}

static int whipe_user(server_data* server) {
  if (!read_byte(server->userData.users[server->userData.read].status, valid)) {
    bzero(&server->userData.users[server->userData.read], sizeof(t_user));
    printf("user %zu was whipe\n", server->userData.read);
  }
  return 0;
}

static int remove_user(server_data* server) {
  FD_CLR(server->userData.users[server->userData.read].fd, &server->as);
  server->nbUser--;
  close(server->userData.users[server->userData.read].fd);
  server->userData.users[server->userData.read].fd = 0;
  rm_message(server, server->userData.read);
  return 0;
}

static char* join_free(char* s1, const char* s2) {
  char* res = NULL;
  size_t s1L = 0, s2L = 0;
  //
  s1L = strlen(s1);
  s2L = strlen(s2);
  res = calloc(s1L + s2L + 1, sizeof(char));
  if (!res) {
    free(s1);
    return NULL;
  }
  while (s2L--) {
    res[s1L + s2L] = s2[s2L];
  }
  while (s1L--) {
    res[s1L] = s1[s1L];
  }
  free(s1);
  return res;
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
    whipe_user(server);
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
    if (select(server->nbUser + 1, &server->rs, &server->ws, NULL, NULL) < 0) {
      perror("select");
      return 1;
    }
    for (server->userData.read = 0; server->userData.read < MAX_USER; server->userData.read++) {
      if(!FD_ISSET(server->userData.users[server->userData.read].fd, &server->rs)) {
        continue ;
      }
      if (server->userData.users[server->userData.read].fd == server->socketFd) {
        socklen_t addr_len = sizeof(server->servaddr);
        const int clId = accept(server->socketFd, (struct sockaddr *)&server->servaddr, &addr_len);
        if (clId > 0) {
          add_user(clId, server);
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