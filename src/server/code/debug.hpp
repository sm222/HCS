#ifndef  DEBUG_HPP
# define DEBUG_HPP

# include "../../dup/setting.h"
# include <stdarg.h>
# include <stdio.h>

void print_error(t_setting* setting, const char* msg, ...);
void print_warning(t_setting* setting, const char* msg, ...);
void print_sucsses(t_setting* setting, const char* msg, ...);


#endif