#ifndef __FLAGS__
# define __FLAGS__


# define FLAG_NAME_LEN 100

typedef struct s_flagValue {
  struct s_flagValue* next;
  int                 flag;
  char                name[FLAG_NAME_LEN];
  char*               value;
} t_flagValue;


# include <stdio.h>

int          fv_add_last(t_flagValue** list, int flag, const char* value);
int          fv_free(t_flagValue** list);
int          fv_set_name(t_flagValue* node, const char* name);
void         fv_print(t_flagValue* list);
ssize_t      fv_list_len(const t_flagValue** head);
int          fv_get_flag(t_flagValue* head, size_t i);
char*        fv_get_value(t_flagValue* head, size_t i);

#endif