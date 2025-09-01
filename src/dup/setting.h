/*
/  /
/  make whit not AI
/  /
*/

#ifndef  SETTING_HPP
# define SETTING_HPP

# ifndef COLORS
#  define COLORS
#  define CS "\001"   //* start code
#  define CE "\002\0" //* end   code
#  define REDCS "\e[31m" CE
#  define GRNCS "\e[32m" CE
#  define YELCS "\e[33m" CE
#  define BLUCS "\e[34m" CE
#  define MAGCS "\e[35m" CE
#  define CYNCS "\e[36m" CE
#  define WHTCS "\e[37m" CE
#  define ORGCS "\e[38;5;202m" CE
#  define PIKCS "\e[38;5;176m" CE
#  define TOXCS "\e[38;5;51m" CE
#  define RESETCS "\e[0m\022" CE
#  define CLECS "\e[1;1H\x1b[2J" CE
#  define GITCS "\e[38;5;82m" CE
#  define GIT_BCS "\e[38;5;94m" CE
#  define MAX_COLORLEN 20
# endif


# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 200
# endif

# include <stdbool.h>

typedef struct s_setting {
  bool   color;
  bool   verbose;
  bool   debug;
} t_setting;

#endif