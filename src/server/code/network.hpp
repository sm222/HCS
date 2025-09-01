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

int init(t_setting* setting);

void send_to_user(std::string msg, t_user& user);
void send_to_user(std::string msg, int fd);


#endif

/*
* https://www.geeksforgeeks.org/linux-unix/how-to-open-ports-in-linux-server-firewall/
*  sudo ufw allow 8080/tcp
*  sudo ufw status numbered
*/