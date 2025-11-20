# include "../../dup/dataType.h"
# include "../../dup/utilse.h"

int print_how_to_use(t_setting* data) {
  put_str_error(data, WHT, "%s: ./%s -'flags' port password\n", data->programeName, data->programeName);
  return 1;
}

int starMain(void* ptr) {
  int error = 0;
  t_setting* data = ptr;
  const t_flagValue* f = data->flagValue;
  const ssize_t nbArgs = fv_list_len(&f);
  if (nbArgs == 0) {
    print_how_to_use(data);
    return 1;
  }
  printf("hello, world\n>%zu\n", nbArgs);
  fv_print(data->flagValue);
  fv_free(&data->flagValue);
  return error;
}