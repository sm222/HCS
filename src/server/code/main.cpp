
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

#include <fstream>
#include <sys/stat.h>

inline bool is_file_here(const std::string& name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

# define FILE_NAME "data/.user"

static bool open_user_file(t_setting* setting) {
  if (!is_file_here(FILE_NAME)) {
    print_warning(setting, "no user file... trying to make one");
  }
  std::ofstream user(FILE_NAME);
  if (user.fail()) {
    print_error(setting, "no file for user");
    return false;
  }
  user.close();
  print_sucsses(setting, "sucsess");
  return true;
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

#include "signal.hpp"

int main(int ac, const char* const* av) {
  t_setting _setting = {
    .color = false,
    .verbose = false,
    .debug = false,
  };
  if (settup(&_setting, ac, av))
    return 1;
  init_signal();
  if (!open_user_file(&_setting))
    return 1;
  t_networkData data;
  if (init(&_setting, data))
    return 1;
  network_loop(data, data.servaddr);
  return 0;
}