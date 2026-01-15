# include "serverUtiles.h"
# include <stdlib.h>

//! wip
int set_user(server_data* server, t_user* user, void* data) {
  (void)server;
  (void)user;
  (void)data;
  return 1;
}

int send_to_user(server_data* server, t_user* u, const char* msg) {
  if (!u || !server) {
    return 2;
  }
  send(u->fd, msg, strlen(msg), 0);
  return 0;
}

int send_str(server_data* server, size_t i, const char* msg) {
  if (i >= MAX_USER)
    return 1;
  t_user* u = server->userData.users + i;
  send_to_user(server, u, msg);
  return 0;
}

int send_str_all(server_data* server, const char* msg, const char* from) {
  for (size_t i  = 1; i < MAX_USER; i++) {
    t_user* u = server->userData.users + i;
    if (u->fd) {
      if (from) {
        send_str(server, i, from);
      }
      send_str(server, i, msg);
    }
  }
  return 0;
}

char* join_free(char* s1, const char* s2) {
  char* res = NULL;
  size_t s1L = 0, s2L = 0;
  //
  s1L = strlen(s1);
  s2L = strlen(s2);
  res = calloc(s1L + s2L + 1, sizeof(char));
  if (!res) {
    free(s1);
    return NULL;
  }
  while (s2L--) {
    res[s1L + s2L] = s2[s2L];
  }
  while (s1L--) {
    res[s1L] = s1[s1L];
  }
  free(s1);
  return res;
}