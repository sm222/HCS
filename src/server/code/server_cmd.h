#ifndef  SERVER_CMD_H
# define SERVER_CMD_H

typedef enum {
  hello = 0,
  exit,
} t_server_cmd;

static const char* const server_cmd[] = {
  "hello:",
  "exit:"
};

#endif //SERVER_CMD_H