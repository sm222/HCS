#ifndef  USER_H
# define USER_H

#include "../../dup/utilse.h"

# define DEF_BUFF_SIZE 255

# define NAME_LEN DEF_BUFF_SIZE
# define IP_LEN   DEF_BUFF_SIZE


//| offline| online
//? 0      | 1
//? 0      | 0
//| awway  | not disturb
//? 0      | 1
//? 1      | 1

typedef enum userStatus {
  online  = 1,
  status  = 2,
  admin   = 4,
  valid   = 8,
} userStatus;

typedef struct t_user {
  unsigned int id;
  int          fd;
  int32_t      status;
  char         name[(NAME_LEN + 1)];
  char         ip[(IP_LEN + 1)];
  char*        msg;
  void*        setting;
} t_user;

#endif