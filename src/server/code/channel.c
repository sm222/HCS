#include "channel.h"
#include <stdlib.h>

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

static void free_block(t_msg_block* head) {
  while (head) {
    t_msg_block* next = head->next;
    while (head->i) {
      head->i--;
      if (head->msgs[head->i].tag == txt) {
        free(head->msgs[head->i].data);
        head->msgs[head->i].data = NULL;
      }
    }
    head = next;
  }
}



int add_msg(channel* chan, t_msg msg) {
  if (!chan) {
    return 2;
  }
  if (chan->block) {

  }
  return 0;
}