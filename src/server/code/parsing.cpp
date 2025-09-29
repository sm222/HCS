#include "parsing.hpp"
# include "utils.hpp"

parsing::parsing(t_networkData& data, userList& listUsers) 
: _data(data), _listUsers(listUsers) {

}

parsing::~parsing(void) {

}

#include <string.h>

int parsing::kick(t_user& user, std::string mgs) {
  send_to_user(mgs, user);
  userListIt it = _listUsers.begin();
  for (; it != _listUsers.end(); it++) {
    if (user.id == it->id) {
      remove_user(user.fd, _data, _listUsers, it);
      break;
    }
  }
  return 1;
}



int parsing::valid_new_user(t_user& user) {
  if (strncmp(PASS_CMD,  user.msg.c_str(), strlen(PASS_CMD)) == 0) {
    if (strcmp(PASS_WORD,  user.msg.c_str() + strlen(PASS_CMD)) == 0) {
      user.status = status_logingIn;
      send_to_user(WELCOME, user);
    }
    else {
      return kick(user, get_random_error(13, 20));
    }
    return 0;
  }
  else {
    kick(user, get_random_error(9, 12));
  }
  return 1;
}

int parsing::setName(t_user& user) {
  const size_t offset = strlen(SET_NAME);
  if (strncmp(SET_NAME,  user.msg.c_str(), offset) == 0) {
    size_t i = offset;
    while (std::isspace(user.msg[i])) { i++; }
    size_t j = 0;
    while (!std::isspace(user.msg[j + i])) { j++; }
    user.name = user.msg.substr(i, j);
    send_to_user("welcome ", user);
    send_to_user(user.name, user);
    send_to_user("\n", user);
    user.status = status_valid;
    return 0;
  }
  send_to_user(get_random_error(20, 22), user);
  return 1;
}

int parsing::join_chan(t_user& user, std::string name) {
  if (user.status < status_valid) {
    send_to_user("user not valid\n", user);
    return 1;
  }
  if (!chans.find(name)) {
    send_to_user("channel not valid\n", user);
    return 2;
  }
  return chans.join(user, name);
}

int parsing::leave_chan(t_user& user, std::string name) {
  if (user.status < status_valid) {
    send_to_user("user not valid\n", user);
    return 1;
  }
  if (!chans.find(name)) {
    send_to_user("user can't leave chan not existing\n", user);
    return 2;
  }
  if (!chans.find_user(name, user)) {
    send_to_user("user can't leave chan is not in\n", user);
    return 3;
  }
  return 0;
}

int parsing::add_cannel(t_user& user, std::string name) {
  return chans.add(name, &user);
}

int parsing::valid_user(t_user& user) {
  //join
  if (strncmp("JOIN:", user.msg.c_str() , 5) == 0) {
    printf("chan name > %s\n", user.msg.c_str() + 5);
    return join_chan(user, user.msg.c_str() + 5);
  }
  //create
  if (strncmp("ADD:", user.msg.c_str() , 4) == 0) {
    printf("add > %s\n", user.msg.c_str() + 4);
    return add_cannel(user, user.msg.c_str() + 4);
  }
  return 0;
}

int parsing::read(t_user& user) {
  if (user.msg.back() == '\n')
    user.msg.back() = '\0';
  if (user.status == status_newUser) {
    return valid_new_user(user);
  }
  else if (user.status == status_logingIn) {
    return setName(user);
  }
  else if (user.status == status_valid) {
    printf("[%d]%s\n", user.id, user.msg.c_str());
    return valid_user(user);
  }
  return 0;
}