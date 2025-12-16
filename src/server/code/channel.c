#include "channel.h"
#include <stdlib.h>

static int get_time_dif(time_t origin) {
  const int t = time(NULL) - origin;
  return t;
}

static int edit_msg(t_msg_block* block, t_msg msg) {
  if (block->i == BLOCK_SIZE) {
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

void free_block(t_msg_block* head) {
  size_t total = 0;
  while (head) {
    t_msg_block* next = head->next;
    printf("free loop\n");
    while (head->i > 1) {
      head->i--;
      if (head->msgs[head->i].tag == txt) {
        total++;
        free(head->msgs[head->i].data);
        head->msgs[head->i].data = NULL;
      }
    }
    free(head);
    head = next;
  }
  printf("messages free: %zu\n", total);
}


static t_msg_block* make_block(void) {
  t_msg_block* r = calloc(1, sizeof(*r));
  r->first = time(NULL);
  return r;
}


static int make_block_last(t_msg_block** head) {
  printf("MAKE\n");
  if (!head)
    return 2;
  if (!(*head)) {
    *head = make_block();
    return *head ? 0 : 1;
  }
  t_msg_block* tmp = *head;
  while (tmp->next) {
    tmp = tmp->next;
  }
  tmp->next = make_block();
  return tmp->next ? 0 : 1;
}

static t_msg_block* get_last_block(channel* chan) {
  if (!chan)
    return NULL;
  t_msg_block* t = chan->block;
  while (t && t->next) {
    t = t->next;
  }
  return t;
}

int add_msg(channel* chan, t_msg msg) {
  if (!chan) {
    return 2;
  }
  if (!chan->block) {
    if (make_block_last(&chan->block))
      return 1;
    edit_msg(chan->block, msg);
  }
  if (edit_msg(chan->block, msg)){
    if (make_block_last(&chan->block))
      return 1;
    t_msg_block* last = get_last_block(chan);
    edit_msg(last, msg);
  }
  return 0;
}