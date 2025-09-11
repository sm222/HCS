#include "network.hpp"
#include "debug.hpp"
#include <arpa/inet.h>
#include "parsing.hpp"
#include "signal.hpp"
#include <string.h>

const int portsList[5] = {
  8080,
  4242,
  8000,
  4200,
  0 // safety
};


int try_bind(t_setting* setting, int& i, int sockfd, struct sockaddr_in* servaddr) {
  (void)setting;
  int loop = 0;
  while (loop < 5 && portsList[loop] != 0) {
    servaddr->sin_port = htons(portsList[loop]); //"put port here");
    if ((bind(sockfd, (const struct sockaddr *)servaddr, sizeof(*servaddr))) < 0) {
      perror("bind");
      print_warning(setting, "%d- - -\n", portsList[loop]);
    }
    else {
      i = loop;
      print_sucsses(setting, "open on:%d", portsList[loop]);
      return 0;
    }
    loop++;
  }
  return 1;
}


void send_to_user(std::string msg, t_user& user) {
  send(user.fd, msg.c_str(), msg.length(), 0);
}


void send_to_user(std::string msg, int fd) {
  send(fd, msg.c_str(), msg.length(), 0);
}


void send_message(t_networkData& data, t_user& from, userList& userData) {
  (void)data;
  userListIt it = userData.begin();
  for (; it != userData.end(); it++) {
    if (it->status != status_server && from.fd != it->fd)
      send(it->fd, from.msg.c_str(), from.msg.length(), 0);
  }
}

t_user add_user(int fd, t_networkData& data, std::list<t_user>& userData) {
  FD_SET(fd, &data.as);
  const char* addres = inet_ntoa(data.servaddr.sin_addr);
  const size_t addres_len = strlen(addres);
  t_user newUser;
  newUser.fd = fd;
  newUser.status = status_newUser;
  newUser.id = ++data.idTotal;
  memcpy(newUser.ip, addres, addres_len + 1);
  data.nuberUser++;
  userData.push_front(newUser);
  send_to_user("welcome: please register on the server\n", newUser);
  return userData.front();
}

void remove_user(int fd, t_networkData& data, userList& userData, userListIt& it) {
  FD_CLR(fd, &data.as);
  userData.erase(it);
  data.nuberUser--;
  close(fd);
}

int network_loop(t_networkData& data, struct sockaddr_in& servaddr, t_setting* setting) {
  std::list<t_user> userData;
  t_user server = {
    .fd = data.sockfd,
    .status = status_server,
    .msg = "",
    .name = "server",
    .id = 0,
    .ip = "self",
  };
  userData.push_front(server);
  parsing passer(data, userData);
  while (!get_stop_sig(false)) {
    data.rs = data.ws = data.as;
    if (select(data.nuberUser + 1, &data.rs, &data.ws, NULL, NULL) < 0) {
      perror("select");
      return 1;
    }
    std::list<t_user>::iterator it = userData.begin();
    for (; it != userData.end(); it++) {
      if (!FD_ISSET(it->fd, &data.rs))
        continue ;
      if (it->fd == data.sockfd) {
        socklen_t addr_len = sizeof(servaddr);
        const int clId = accept(data.sockfd, (struct sockaddr *)&servaddr, &addr_len);
        if (clId >= 0) {
          const t_user& newUser = add_user(clId, data, userData);
          print_notify(setting, "[id > %d|addr > %s]new user join", newUser.id, newUser.ip);
          break;
        }
      }
      else {
        char buff[1001];
        ssize_t rb = recv(it->fd, buff, 1000, 0);
        if (rb <= 0) {
          char leaveB[100];
          snprintf(leaveB, 99, "[id > %d|addr > %s]%s:left\n", it->id, it->ip, it->name.c_str());
          fprintf(stderr, "%s", leaveB);
          it->msg = leaveB;
          send_message(data, *it, userData);
          remove_user(it->fd, data, userData, it);
          it = userData.begin();
        }
        else {
          buff[rb] = 0;
          it->msg += buff;
          if (it->msg.length() > 0 && it->msg.back() == '\n') {
            if (passer.read(*it))
              it = userData.begin();
            it->msg.clear();
          }
        }
      }
    }
  }
  return 1;
}

int init(t_setting* setting, t_networkData& data) {
  bzero(&data.servaddr, sizeof(data.servaddr));
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket");
    return 1;
  }
  data.servaddr.sin_family = AF_INET;
  data.servaddr.sin_addr.s_addr = INADDR_ANY;
  int i = 0;
  if (try_bind(setting, i, sockfd, &data.servaddr)) {
    return 1;
  }
  if (listen(sockfd, MAX_USER) < 0) {
    perror("listen");
    return 1;
  }
  print_sucsses(setting, "suscess: ready to run");
  data.nuberUser = sockfd;
  data.sockfd = sockfd;
  data.idTotal = 1;
  FD_ZERO(&data.as);
  FD_SET(sockfd, &data.as);
  return 0;
}

