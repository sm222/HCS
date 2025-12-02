#ifndef  SERVER_H
# define SERVER_H

# include <netdb.h>
# include <sys/socket.h>
# include <netinet/in.h>
//
# include "user.h"


# define MAX_PASS_LEN 255
# define MAX_USER     200

typedef struct {
  t_user*      users;
  size_t       size;
  size_t       regist;
  size_t       read;
} user_data;

# define DEF_PORT 8080

typedef struct server_data {
  int          port;
  char         password[(MAX_PASS_LEN + 1)];
  //
  fd_set       rs, ws, as;
  unsigned int nbUser;
  int          socketFd;
  unsigned int ids;
  struct sockaddr_in servaddr;
  //
  user_data    userData;
} server_data;


int init_server(t_setting* data, server_data* server, size_t nbArgs);



#endif