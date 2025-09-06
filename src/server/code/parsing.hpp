#ifndef  PARSING_HPP
# define PARSING_HPP

# include "user.hpp"
# include "channels.hpp"
# include "network.hpp"

# define PASS_CMD "PASS:"
# define PASS_WORD "abc"
//PASS:hello_world

# define WELCOME "please chose a user name\n"

# define SET_NAME "NAME:"

class parsing {
  public:
    parsing(t_networkData& data, userList& listUsers);
    ~parsing(void);
    int read(t_user& user);
  private:
    int valid_new_user(t_user& user);
    int kick(t_user& user, std::string  msg = "rip bozo");
    //
    int setName(t_user& user);
    //
    channel chans;
    t_networkData& _data;
    userList&      _listUsers;
};



#endif