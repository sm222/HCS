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
  channel_it->users.push_back(&user);
  message(name, "new user join channel\n");
  /*
  * notify here
  */
  return 0;
}

int channel::remove(std::string name) {
  if (name.empty())
    return 1;
  std::vector<t_channelData>::iterator it = _get(name);
  if (it != channels.end()) {
    /*
    * remove all user
    */
    channels.erase(it);
    return 0;
  }
  return 2;
}

int channel::message(std::string channel, std::string msg, t_user* from) {
  (void)from;
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

int  channel::logMessage(std::string name) {
  if (name.empty())
    return 1;
  
  const int fd = open(name.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
  if (fd < 0)
    return 2;
}