#ifndef  NETWORD_HPP
# define NETWORD_HPP

# include "../../dup/setting.h"
# include "user.hpp"
# include <netdb.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <strings.h>
# include <stdio.h>
# include <unistd.h>

# define MAX_USER 10

typedef struct s_networkData {
  int  port;
  fd_set rs, ws, as;
  int          nuberUser;
  int          sockfd;
  unsigned int idTotal;
  struct sockaddr_in servaddr;
} t_networkData;

int init(t_setting* setting, t_networkData& data);
int network_loop(t_networkData& data, struct sockaddr_in& servaddr, t_setting* setting);

void send_to_user(std::string msg, t_user& user);
void send_to_user(std::string msg, int fd);
void send_to_user(const char* str, t_user& user);
void send_to_user(const char* str, int fd);

# include <list>
  typedef std::list<t_user> userList;
  typedef userList::iterator userListIt;

// add and remove
void remove_user(int fd, t_networkData& data, userList& userData, userListIt& it);

#endif

/*
* https://www.geeksforgeeks.org/linux-unix/how-to-open-ports-in-linux-server-firewall/
*  sudo ufw allow 8080/tcp
*  sudo ufw status numbered
*/