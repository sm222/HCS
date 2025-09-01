#ifndef  CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include "user.hpp"
# include "network.hpp"

typedef struct t_channelData {
  std::string          name;
  size_t               usersNumber;
  std::vector<t_user*> users;

} t_channelData;

class channel {
  public:
    channel();
    ~channel();
    int  add(std::string name, t_user* user = nullptr);
    int  remove(std::string name);
    bool find(std::string name) const;
    //
    int  message(std::string channel, std::string msg, t_user* from = nullptr);
    //
    int  join(t_user& user, std::string name);
    int  leave(t_user& user, std::string name);
  private:
    std::vector<t_channelData>::iterator _get(std::string name);
    std::vector<t_channelData> channels;
};

#endif