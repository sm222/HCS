
#include "../../dup/dataType.h"

#include "serverMain.h"

int addValue(void* ptr, const char* v) {
  static int i = 0;
  if (!ptr || !v)
    return -1;
  t_setting* data = ptr;
  const int e = fv_add_last(&data->flagValue, i++, v);
  return e < 0 ? 1 : 0;
}

int setStart(void* ptr) {
  if (!ptr) {
    write(STDERR_FILENO, "no ptr\n", 7);
    return 1;
  }
  t_setting* data = ptr;
  data->avFt      = addValue;
  data->programFt = starMain;
  return 0;
}