#include "channels.hpp"


channel::channel() {

}

channel::~channel() {
  
}

int  channel::disc(t_user& user) {
  int nb = 0;
  std::vector<t_channelData>::iterator it = channels.begin();
  for ( ; it != channels.end(); it++) {
    nb += !remove_user(it->name, user);
  }
  return nb;
}

bool channel::find(std::string name) const {
  std::vector<t_channelData>::const_iterator it;
  for (it = channels.begin(); it != channels.end(); it++) {
    if (it->name == name)
      return true;
  }
  return false;
}

bool channel::find_user(std::string name, t_user& user) {
  std::vector<t_channelData>::iterator it = _get(name);
  if (it == channels.end())
    return false;
  std::vector<t_user*>::iterator user_it = it->users.begin();
  for ( ; user_it != it->users.end(); user_it++) {
    if ((*user_it)->id == user.id)
      return true;
  }
  return false;
}

bool channel::find_user(std::vector<t_channelData>::iterator it, t_user& user) {
  if (it == channels.end())
    return false;
  std::vector<t_user*>::iterator user_it = it->users.begin();
  for ( ; user_it != it->users.end(); user_it++) {
    if ((*user_it)->id == user.id)
      return true;
  }
  return false;
}

int channel::add(std::string name, t_user* user) {
  if (name.empty())
    return 1;
  if (find(name))
    return 2;
  t_channelData chan;
  chan.name = name;
  chan.usersNumber = user ? 1 : 0;
  if (user)
    chan.users.push_back(user);
  channels.push_back(chan);
  return 0;
}

int channel::join(t_user& user, std::string name) {
  if (user.status < status_valid)
    return 1;
  std::vector<t_channelData>::iterator channel_it = _get(name);
  if (channel_it == channels.end()) {
    send_to_user("ERR:NCN\n", user);
    return 2;
  }
  std::vector<t_user*>::iterator users_it = channel_it->users.begin();
  for (; users_it != channel_it->users.end(); users_it++) {
    if (*(*users_it) == user) {
      char buff[200];
      snprintf(buff, 199, "ERR:ARI %s %s\n", user.name.c_str(), name.c_str());
      send_to_user(buff, user);
      return 3;
    }
  }
  // add if chan lock ?
  char msg[200];
  snprintf(msg, 199, "new user %s join %s\n", user.name.c_str(), channel_it->name.c_str());
  message(name, msg);
  users_it = channel_it->users.begin();
  send_to_user("USERLIST: ", user);
  for (; users_it != channel_it->users.end(); users_it++) {
    if (user.id != (*users_it)->id) {
      snprintf(msg, 199, "%s ", (*users_it)->name.c_str());
      send_to_user(msg, user);
    }
  }
  send_to_user("\n", user);
  channel_it->users.push_back(&user);
  return 0;
}

int channel::remove(std::string name) {
  if (name.empty())
    return 1;
  std::vector<t_channelData>::iterator it = _get(name);
  if (it != channels.end()) {
    char msg[200];
    snprintf(msg, 199, "leave:%s channel was close", name.c_str());
    message(name, msg);
    channels.erase(it);
    return 0;
  }
  return 2;
}

int  channel::remove_user(std::string name, t_user& user) {
  std::vector<t_channelData>::iterator it = _get(name);
  if (!find_user(it, user))
    return 1;
  std::vector<t_user*>::iterator users_it = it->users.begin();
  for ( ; users_it != it->users.end(); users_it++) {
    if ((*users_it)->id == user.id) {
      it->users.erase(users_it);
      break ;
    }
  }
  return 0;
}


int channel::message(std::string channel, std::string msg, t_user* from) {
  if (channel.empty() || msg.empty())
    return 1;
  std::vector<t_channelData>::iterator it = _get(channel);
  if (it == channels.end())
    return 2;
  std::vector<t_user*>::iterator user_it = it->users.begin();
  t_message m;
  SET_MESSAGE_TIME(m.time);
  m.txt = msg;
  (void)from;
  m.name[0] = 0;
  it->messages.push_back(m);
  for (; user_it != it->users.end(); user_it++) {
    send_to_user(msg, *(*user_it));
  }
  return 0;
}

int channel::list(std::string channel) {
  std::vector<t_channelData>::iterator it = _get(channel);
  if (it == channels.end())
    return 1;
  std::vector<t_user*>::const_iterator l = it->users.begin();
  for (; l != it->users.end(); l++) {
    printf("* - %s\n", (*l)->name.c_str());
  }
  return 0;
}

std::vector<t_channelData>::iterator channel::_get(std::string name) {
  std::vector<t_channelData>::iterator it;
  for (it = channels.begin(); it != channels.end(); it++) {
    if (name == it->name)
      return it;
  }
  return channels.end();
}

# include <fcntl.h>
# include <string.h>

int  channel::logMessage(std::string name) {
  if (name.empty())
    return 1;
  std::vector<t_channelData>::const_iterator it;
  it = _get(name);
  if (it == channels.end())
    return 2;
  const int fd = open(name.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
  if (fd < 0)
    return 3;
  std::list<t_message>::const_iterator msg_it = it->messages.begin();
  write(fd, "message:", 9);
  write(fd, name.c_str(), name.length());
  write(fd, "\n", 1);
  for (; msg_it != it->messages.end(); msg_it++) {
    write(fd, msg_it->time, strlen(msg_it->time));
    write(fd, ":", 1);
    write(fd, msg_it->txt.c_str(), msg_it->txt.length());
  }
  close(fd);
  return 0;
}