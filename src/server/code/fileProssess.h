#ifndef  _FILE_PROSSESS_H_
# define _FILE_PROSSESS_H_

# include "server.h"

typedef struct {
  ssize_t fileSize;
  char**  rawFile;
  size_t  arraySize;
  int fd;
} t_file;

t_file* open_file(const char* name);
void    draw_file(t_file* file);
void    free_file(t_file** file);



#endif
