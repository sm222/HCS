#ifndef  CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include "user.hpp"
# include "network.hpp"
# include "debug.hpp"

//Wed Sep 10 16:23:32 2025 [28]


const char* const MessageType[] = {
  "ERR",
  "PASS",
  "NAME",
  "ADD",
  "JOIN",
  NULL
};

const char* const MessageErr[] = {
  "ARI", //all ready in
  "NCN", //no chanell name
  NULL
};



typedef struct t_message {
  t_user*      form;
  std::string  txt;
  char         time[26];
} t_message;



typedef struct t_channelData {
  std::string           name;
  size_t                usersNumber;
  std::vector<t_user*>  users;
  std::list<t_message>  messages;
} t_channelData;

# define SET_MESSAGE_TIME(msg)   set_time(msg, 25)

class channel {
  public:
    channel();
    ~channel();
    int  add(std::string name, t_user* user = nullptr);
    int  remove(std::string name);
    int  remove_user(std::string name, t_user& user);
    bool find(std::string name) const;
    bool find_user(std::string name, t_user& user);
    bool find_user(std::vector<t_channelData>::iterator it, t_user& user);
    //
    int  message(std::string channel, std::string msg, t_user* from = nullptr);
    //
    int  join(t_user& user, std::string name);
    int  leave(t_user& user, std::string name);
    int  logMessage(std::string name);
  private:
    std::vector<t_channelData>::iterator _get(std::string name);
    std::vector<t_channelData> channels;
};

#endif