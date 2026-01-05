#ifndef  CHANNEL_H
# define CHANNEL_H

# include "../../dup/utilse.h"
# include "user.h"
# include <time.h>

# ifdef DEV
#  define BLOCK_SIZE 10
# else
#  define BLOCK_SIZE 255
# endif

typedef enum {
  cs_open           = 1,
  cs_user_message   = 2,
  cs_user_can_join  = 4,
  cs_user_auto_join = 8,
} channel_status;

typedef enum {
  none,
  txt,
  img,
  audio,
  file,
} msg_type;

typedef struct s_msg {
  msg_type     tag;
  int          timeDif;
  size_t       size;
  void*        data;
} t_msg;

typedef struct s_msg_block {
  struct s_msg_block* next;
  time_t              first;
  t_msg               msgs[BLOCK_SIZE];
  int16_t             i;
} t_msg_block;

typedef struct s_channel {
  struct s_channel* next;
  struct s_channel* prev;
  const char*       name;
  int32_t           status;
  t_msg_block*      block;
  size_t            maxUser;
} channel;

void free_block(t_msg_block* head);
int add_msg(channel* chan, t_msg msg);

#endif
