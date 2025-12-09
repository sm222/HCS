#ifndef  CHANNEL_H
# define CHANNEL_H

#include "../../dup/utilse.h"
# include "user.h"


typedef enum {
  cs_open           = 1,
  cs_user_message   = 2,
  cs_user_can_join  = 4,
  cs_user_auto_join = 8,
} channel_status;

typedef enum {
  txt,
  img,
  audio,
  file,
} msg_type;

typedef struct s_msg {
  msg_type     tag;
  size_t       size;
  void*        data;
} t_msg;

typedef struct s_msg_block {
  struct s_msg_block* next;
  t_msg               msgs[255];
  int8_t              i;
} t_msg_block;

typedef struct s_channel {
  struct s_channel* next;
  struct s_channel* prev;
  const char*       name;
  int32_t           status;
  size_t            maxUser;

} channel;

#endif
