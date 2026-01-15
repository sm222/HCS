#ifndef  SERVER_H
# define SERVER_H

# include <netdb.h>
# include <sys/socket.h>
# include <netinet/in.h>
// # include <poll.h>
//
# include "channel.h"
# include "user.h"

# define VERTION "0.0.1"

# define MAX_PASS_LEN 255
# define MAX_USER     200
# define MAX_PROFILE  (MAX_USER / 2)
# define IP_BUFF_LEN  30

typedef struct {
  t_user*      users;
  //size_t       size; // array size not use rn
  size_t       regist;
  size_t       read;
} user_data;

# define DEF_PORT 8080

# define READ_BUFF_SIZE 1000

typedef struct server_data {
  int          port;
  char         password[(IP_BUFF_LEN + 1)];
  //
  //struct pollfd fds[MAX_USER]; // stay on select for now
  fd_set        rs, ws, as;
  unsigned int  nbUser;
  int           socketFd;
  int           maxFd;
  unsigned int  ids;
  struct sockaddr_in servaddr;
  //
  channel      def;
  user_data    userData;
  t_user       profile[MAX_PROFILE];
} server_data;


int init_server(t_setting* data, server_data* server, size_t nbArgs);
int setup_server_user(server_data* server);
int server_clean_up(t_setting* data, server_data* server);

int network_loop(server_data* server);

int set_user_data(void* data, t_user* user);

int send_str_all(server_data* server, const char* msg, const char* from);
int send_str(server_data* server, size_t i, const char* msg);
int send_to_user(server_data* server, t_user* u, const char* msg);


#endif