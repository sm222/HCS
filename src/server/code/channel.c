#include "channel.h"

static int get_time_dif(time_t origin) {
  const int t = time(NULL) - origin;
  return t;
}

int edit_msg(t_msg_block* block, t_msg msg) {
  if (block->i == 255) {
    return 1;
  }
  block->msgs[block->i] = (t_msg){
    .timeDif = get_time_dif(block->first),
    .data = msg.data,
    .size  = msg.size,
    .tag = msg.tag
  };
  block->i++;
  return 0;
}

int add_msg(channel* chan, t_msg msg) {
  if (!chan) {
    return 2;
  }
  return 0;
}