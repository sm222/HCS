#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

# define NL '\n'
# define B_SIZE 101

#  define BUFFER_SIZE 100

typedef struct s_info {
  char  *tmp;
  char  *tmp2;
  char  readtmp[BUFFER_SIZE + 1];
  int    rv;
  size_t  cut;
}  t_info;

static void* ft_sfree(void* ptr) {
  free(ptr);
  return NULL;
}

static char  *ft_strjoin(char *sfree, char *s2) {
  size_t s1_i = strlen(sfree ? sfree : "");
  size_t s2_i = strlen(s2 ? s2 : "");
  char* new = calloc(s1_i + s2_i + 1, sizeof(char));

  if (!new) {
    sfree = ft_sfree(sfree);
    return (NULL);
  }
  while (s1_i + s2_i-- > s1_i)
    new[s1_i + s2_i] = s2[s2_i];
  while (s1_i--)
    new[s1_i] = sfree[s1_i];
  if (*new == 0)
    new = ft_sfree(new);
  return (ft_sfree(sfree), new);
}

static char  *ft_tiny_split(char *s, size_t *cut) {
  char  *new = NULL;
  size_t  i = 0;

  while (s[i])
    if (s[i++] == '\n')
      break ;
  new = calloc(i + 1, sizeof(char));
  if (!new)
    return (new = ft_sfree(new));
  *cut = i;
  while (i--)
    new[i] = s[i];
  return (new);
}

static char  ft_find(char *s) {
  size_t  i = 0;

  while (s && s[i]) {
    if (s[i] == '\n')
      return ('\n');
    i++;
  }
  return ('0');
}

static char  *safe_return(char **book, t_info *t_val) {
  t_val->tmp = ft_tiny_split(*book, &t_val->cut);
  if (!t_val->tmp) {
    *book = ft_sfree(*book);
    return (NULL);
  }
  t_val->tmp2 = *book;
  *book = ft_strjoin(NULL, *book + t_val->cut);
  t_val->tmp2 = ft_sfree(t_val->tmp2);
  return (t_val->tmp);
}

static char  *get_next_line(int fd) {
  static char  *book = NULL;
  t_info    t_val;

  if (fd < 0 || BUFFER_SIZE <= 0 || fd > 255)
    return (book = ft_sfree(book));
  if (!book)
    book = calloc(1, sizeof(char));
  if (!book)
    return (NULL);
  t_val.rv = 0;
  while (ft_find(book) == '0') {
    bzero(t_val.readtmp, BUFFER_SIZE + 1);
    t_val.rv = read(fd, t_val.readtmp, BUFFER_SIZE);
    if (t_val.rv <= 0)
      break ;
    book = ft_strjoin(book, t_val.readtmp);
    if (!book)
      return (NULL);
  }
  if (t_val.rv == -1 || (t_val.rv <= 0 && *book == 0)) {
    book = ft_sfree(book);
    return (NULL);
  }
  return (safe_return(&book, &t_val));
}

# include "fileProssess.h"


static int read_file(t_file* file) {
  const size_t startSize = 10;
  file->rawFile = calloc(startSize, sizeof(char*));
  size_t line = 0;
  size_t rawFileSize = startSize;
  char* l = "line";
  while (l) {
    l = get_next_line(file->fd);
    file->rawFile[line] = l;
    file->fileSize += strlen(l ? l : "");
    line++;
    if (line > rawFileSize / 2) {
      rawFileSize *= 2;
      file->rawFile = realloc(file->rawFile, sizeof(char*) * rawFileSize);
    }
  }
  file->arraySize = line;
  return 0;
}

t_file* open_file(const char* name) {
  t_file* file = calloc(1, sizeof(*file));
  if (!file) {
    perror("calloc");
    return NULL;;
  }
  file->fd = open(name, O_RDONLY);
  if (file->fd == -1) {
    perror("open");
    return file;
  }
  if (read_file(file)) {
    perror("read_file");
  }
  return file;
}

void draw_file(t_file* file) {
  if (!file) {
    return ;
  }
  for (size_t i = 0; i < file->arraySize; i++) {
    printf("[%zu]%s", i, file->rawFile[i]);
  }
}

void free_file(t_file** file) {
  if (!file || !(*file)) {
    return ;
  }
  close((*file)->fd);
  for (size_t i = 0; i < (*file)->arraySize; i++) {
    free((*file)->rawFile[i]);
  }
  free((*file)->rawFile);
  free(*file);
  *file = NULL;
}

int get_file_type(const t_file* file) {
  if (!file || file->arraySize <= 1) {
    return invalid_file;
  }
  const char* firstLine = file->rawFile[0];
  if (strncmp(firstLine, "#TYPE:", 6) == 0) {
    if (strcmp(firstLine + 6, "USER\n") == 0)
      return user_file;
  }
  return invalid_file;
}

/*
int set_user_from_file(server_data* server, server_interface ft) {
  ft(server, NULL, NULL);
}
*/