
#include "../../dup/setting.h"
#include "debug.hpp"
#include "network.hpp"

#include <stdio.h>
#include <string>

static int add_setting(t_setting* setting, char c, size_t i) {
  if (c == '-' && i == 0)
    return 1;
  if (c == 'd') {
    setting->debug = true;
  }
  else if (c == 'c') {
    setting->color = true;
  }
  else if (c == 'v') {
    setting->verbose = true;
  }
  else {
    print_error(setting, "flag unknow: %c\n", c);
    return 1;
  }
  return 0;
}


static int settup(t_setting* setting, int ac, const char* const* av) {
  int i = 1;
  while (i < ac) {
    std::string txt = av[i];
    if (txt.length() < 2) {
      fprintf(stderr, "invalid argument: %s\n", txt.c_str());
      return 1;
    }
    size_t j = 0;
    while (j < txt.length()) {
      add_setting(setting, txt[j], j);
      ++j;
    }
    ++i;
  }
  return 0;
}


int main(int ac, const char* const* av) {
  (void)ac;
  (void)av;
  t_setting _setting = {
    .color = false,
    .verbose = false,
    .debug = false,
  };
  if (settup(&_setting, ac, av))
    return 1;
  init(&_setting);
  return 0;
}