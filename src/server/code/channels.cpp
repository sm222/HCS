#include "channels.hpp"


channel::channel() {

}

channel::~channel() {
  
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
  if (channel_it == channels.end())
    return 2;
  std::vector<t_user*>::iterator users_it = channel_it->users.begin();
  for (; users_it != channel_it->users.end(); users_it++) {
    if (*(*users_it) == user) {
      return 3;
    }
  }
  // add if chan lock ?
  char msg[200];
  snprintf(msg, 199, "new user %s join channel\n", user.name.c_str());
  message(name, msg);
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
  m.form = from;
  m.txt = msg;
  it->messages.push_back(m);
  for (; user_it != it->users.end(); user_it++) {
    send_to_user(msg, *(*user_it));
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