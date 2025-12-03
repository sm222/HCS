# include "../../dup/dataType.h"
# include "server.h"
# include <stdlib.h>
# include <arpa/inet.h>



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
  if (listen(server->socketFd, 10) < 0) {
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


int server_clean_up(t_setting* data, server_data* server) {
  free(server->userData.users);
  fv_free(&data->flagValue);
  return 0;
}

static int add_user(int fd, server_data* server) {
  FD_SET(fd, &server->as);
  server->ids++;
  server->nbUser++;
  const char* addres = inet_ntoa(server->servaddr.sin_addr);
  const size_t addres_len = strlen(addres);
  unsigned int i  = 1;
  printf("add nb:%u\nid:%u\n", server->nbUser, server->ids);
  while (i < MAX_USER) {
    if (server->userData.users[i].id == 0)
      break ;
    i++;
  }
  if (i == MAX_USER) {
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
  return 0;
}

static int remove_user(server_data* server) {
  FD_CLR(server->userData.users[server->userData.read].fd, &server->as);
  server->nbUser--;
  close(server->userData.users[server->userData.read].fd);
  server->userData.users[server->userData.read].fd = 0;
  free(server->userData.users[server->userData.read].msg);
  server->userData.users[server->userData.read].msg = NULL;
  return 0;
}

static int manage_user(server_data* server) {
  char buff[READ_BUFF_SIZE + 1];
  const ssize_t readByte = recv(server->userData.users[server->userData.read].fd, buff , READ_BUFF_SIZE, 0);
  if (readByte <= 0) {
    remove_user(server);
    printf("dc\n");
    return 0;
  }
  buff[readByte] = 0;
  printf("%s:%s\n", server->userData.users[server->userData.read].ip, buff);
  return 0;
}

int network_loop(server_data* server) {
  while (true) {
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
        manage_user(server);
      }
    }
  }
  return 0;
}