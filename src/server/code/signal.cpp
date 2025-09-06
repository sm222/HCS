#include "signal.hpp"

bool get_stop_sig(bool set) {
  static bool run = false;
  if (set)
    run = true;
  return run;
}

static void get_signal(int sig) {
  (void)sig;
  printf("\nterminate\n");
  get_stop_sig(true);
}

void init_signal(void) {
  signal(SIGINT, get_signal);
  signal(SIGTERM, get_signal);
  signal(SIGQUIT, get_signal);
  signal(SIGPIPE, SIG_IGN);
}