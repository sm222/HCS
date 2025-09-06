#ifndef  USER_HPP
# define USER_HPP

# include <string>

enum {
  status_server = 0,
  status_newUser,
  status_logingIn,
  status_logingInUser, // get unic user and password
  status_valid,
  status_admin,
  status_superAdmin,
};

typedef struct s_user {
  int           fd;
  unsigned int  status;
  std::string   msg;
  std::string   name;
  unsigned int  id;
  bool operator==(s_user& rhs) {
    return rhs.name == this->name;
  }
} t_user;


#endif
