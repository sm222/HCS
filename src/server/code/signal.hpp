#ifndef  SIGNAL_R_H
# define SIGNAL_R_H

#include <csignal>
#include <stdio.h>

void init_signal(void);
bool get_stop_sig(bool set);

#endif