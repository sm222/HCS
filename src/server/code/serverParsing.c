# include "serverParsing.h"
# include <stdlib.h>
# include "user.h"

static bool ask_password(server_data* server, t_user* u) {
  if (strcmp(server->password, u->msg) == 0) {
    set_byte(&u->status, valid, true);
    return 1;
  }
  return 0;
}


static int is_user_valid(server_data* server, t_user* u) {
  if (read_byte(u->status, valid))
    return 0;
  const bool pass = ask_password(server, u);
  return pass ? 0 : 2;
}

int dispatch(server_data* server) {
  t_user* ref = server->userData.users + server->userData.read;
  //printf("user:%d -> %s\n", ref->fd, ref->msg);
  if (strcmp("exit", ref->msg) == 0) { //! only for dev
    free(ref->msg);
    ref->msg = NULL;
    return 1;
  }
  if (is_user_valid(server, ref))
    return 2;
  t_msg m = {
    txt,
    0,
    strlen(ref->msg),
    ref->msg,
  };
  ref->msg = NULL;
  add_msg(&server->def, m);
  return 0;
}