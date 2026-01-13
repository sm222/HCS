#ifndef  _FILE_PROSSESS_H_
# define _FILE_PROSSESS_H_

# include "server.h"

typedef struct {
  ssize_t fileSize;
  char**  rawFile;
  size_t  arraySize;
  int fd;
} t_file;

typedef enum {
  invalid_file = -1,
  user_file = 0,
} t_file_type;

t_file* open_file(const char* name);
void    draw_file(t_file* file);
void    free_file(t_file** file);
int     get_file_type(const t_file* file);

typedef  int(*server_interface)(server_data*, t_user*, void*);

#endif
