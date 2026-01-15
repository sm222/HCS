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

typedef enum {
  f_is_ok    = 1,
  f_is_read  = 2,
  f_is_write = 4,
} file_info;

int32_t test_file_perms(const char* fileName);
void    print_file_error(int32_t file, const char* fileName);
//
t_file* open_file(const char* name);
void    print_file(t_file* file);
char*   read_line(const t_file* file, size_t line);
void    close_file(t_file** file);
int     get_file_type(const t_file* file);

typedef  int(*server_interface)(server_data*, t_user*, void*);

#endif
