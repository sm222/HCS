
#include "../../dup/dataType.h"

#include "serverMain.h"


int setStart(void* ptr) {
  if (!ptr) {
    write(STDERR_FILENO, "no ptr\n", 7);
    return 1;
  }
  t_setting* data = ptr;
  printf("setup work:%s\n", data->av[0]);
  data->programFt = starMain;
  return 0;
}