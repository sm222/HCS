# include "../../dup/dataType.h"
# include "server.h"
# include <stdlib.h>


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
  const char* password = fv_get_value(data->flagValue, 0);
  printf("password: %s\n", password);
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
  printf("server about to start\n");
  return 0;
}