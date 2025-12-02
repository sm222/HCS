# include "flags.h"
# include "utilse.h"
# include <string.h>
# include <strings.h>
# include <stdlib.h>


static t_flagValue* fv_add(int flag, const char* value) {
  t_flagValue* f = calloc(1, sizeof(*f));
  if (f) {
    f->flag = flag;
    bzero(f->name, FLAG_NAME_LEN * sizeof(char));
    f->value = d__strdup(value);
    if (!f->value) {
      free(f);
      f = NULL;
    }
  }
  return f;
}

int fv_add_last(t_flagValue** list, int flag, const char* value) {
  if (!list)
    return -1;
  if (!(*list)) {
    *list = fv_add(flag, value);
    if (!(*list))
      return -2;
  }
  else {
    t_flagValue *tmp = (*list);
    for (; tmp && tmp->next; tmp = tmp->next) { }
    tmp->next = fv_add(flag, value);
    if (!tmp->next)
      return -2;
  }
  return 0;
}

int fv_free(t_flagValue** list) {
  if (!list)
    return -1;
  for (t_flagValue* tmp = (*list); tmp; ) {
    free(tmp->value);
    t_flagValue *t = tmp->next;
    free(tmp);
    tmp = t;
  }
  return 0;
}

int fv_set_name(t_flagValue* node, const char* name) {
  if (!node)
    return 1;
  const size_t l = strlen(name);
  memcpy(node->name, name, l > FLAG_NAME_LEN - 1 ? FLAG_NAME_LEN -1 : l);
  node->name[FLAG_NAME_LEN - 1] = 0;
  return 0;
}

ssize_t fv_list_len(const t_flagValue** head) {
  if (!head)
    return -1;
  ssize_t i = 0;
  const t_flagValue* ptr = *head;
  while (ptr) {
    i++;
    ptr = ptr->next;
  }
  return i;
}

char* fv_get_value(t_flagValue* head, size_t i) {
  char* v = NULL;
  if (!head)
    return v;
  t_flagValue* tmp = head;
  while (i && tmp) {
    i--;
    tmp = tmp->next;
  }
  if (tmp)
    v = tmp->value;
  return v;
}

# include <stdio.h>
void fv_print(t_flagValue* list) {
  for ( ; list; list = list->next) {
    printf("[%d]%s\n", list->flag, list->value);
  }
}