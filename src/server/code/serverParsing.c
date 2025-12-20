# include "serverParsing.h"
# include <stdlib.h>



int dispatch(server_data* server) {
  (void)server;
  t_user* ref = server->userData.users + server->userData.read;
  //printf("user:%d -> %s\n", ref->fd, ref->msg);
  if (strcmp("exit", ref->msg) == 0) {
    free(ref->msg);
    ref->msg = NULL;
    return 1;
  }
  t_msg m = {
    txt,
    0,
    strlen(ref->msg),
    strdup(ref->msg),
  };
  add_msg(&server->def, m);
  free(ref->msg);
  ref->msg = NULL;
  return 0;
}