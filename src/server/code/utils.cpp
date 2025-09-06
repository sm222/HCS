#include <stdlib.h>
#include <stdio.h>

int get_random_value(int min, int max) {
  return rand() % (max - min + 1) + min;
}

const char* get_random_value_txt(int min, int max) {
  static char str[100];
  snprintf(str, 99, "%d", get_random_value(min, max));
  return str;
}

const char* get_random_error(int min, int max) {
  static char str[100];
  snprintf(str, 99, "error:%d\n", get_random_value(min, max));
  return str;
}
