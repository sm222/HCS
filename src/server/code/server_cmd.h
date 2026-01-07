#ifndef  SERVER_CMD_H
# define SERVER_CMD_H


# define LEN_OF_CODE 2


static const char* const server_cmd[] = {
  "00:hello:"  ,
  "01:exit:"   ,
  "02:login"   ,
  "03:logout"  ,
  "04:makeUser",
};

#endif //SERVER_CMD_H