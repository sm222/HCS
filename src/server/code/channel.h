#ifndef  CHANNEL_H
# define CHANNEL_H

#include "../../dup/utilse.h"
# include "user.h"


typedef enum {
  cs_open = 1,

} channel_status;

typedef struct s_channel {
  struct s_channel* next;
  int32_t           status;
  
} channel;

#endif
