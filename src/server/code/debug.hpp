#ifndef  DEBUG_HPP
# define DEBUG_HPP

# include "../../dup/setting.h"
# include <stdarg.h>
# include <stdio.h>

void set_time(char* const txt, size_t len);

void print_error(t_setting*   setting, const char* msg, ...);
void print_warning(t_setting* setting, const char* msg, ...);
void print_sucsses(t_setting* setting, const char* msg, ...);
void print_notify(t_setting*  setting, const char* msg, ...);


#endif