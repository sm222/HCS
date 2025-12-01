# include "../../dup/dataType.h"
# include "../../dup/utilse.h"

static int print_how_to_use(t_setting* data) {
  put_str_error(data, WHT, "%s: -'flags' password port", data->programeName);
  return 1;
}

static int test_nb_arg(t_setting* data , const ssize_t nb) {
  if (nb != 2) {
    print_how_to_use(data);
    return 1;
  }
  return 0;
}

int starMain(void* ptr) {
  int error = 0;
  t_setting* data = ptr;
  const t_flagValue* f = data->flagValue;
  const ssize_t nbArgs = fv_list_len(&f);
  if (test_nb_arg(data, nbArgs)) {
    return 1;
  }
  printf("hello, world\n>%zu\n", nbArgs);
  fv_print(data->flagValue);
  fv_free(&data->flagValue);
  return error;
}