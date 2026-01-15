# include "serverParsing.h"
# include <stdlib.h>
# include <ctype.h>
# include "user.h"
# include "server_cmd.h"
# include <stdarg.h>

void server_say(const size_t bSize, server_data* server, t_user* u, const char* msg, ...) {
  if (bSize == 0)
    return;
  va_list ap;
  char out[bSize];
  va_start(ap, msg);
  bzero(out, bSize);
  vsprintf(out, msg, ap);
  send_to_user(server, u, out);
  va_end(ap);
}

static void server_say_hello(server_data* server, t_user* u) {
  time_t rawtime = time(NULL);
  struct tm* timeinfo = localtime(&rawtime);
  const char* s = asctime(timeinfo);
  const size_t s_len = strlen(s);
  server_say(301, server, u, "00:hello %.*s HCS %s\n", (int)s_len - 1, s, VERTION);
}

static bool ask_password(server_data* server, t_user* u) {
  if (strcmp(server->password, u->msg) == 0) {
    const size_t l = strlen(u->msg);
    memset(u->msg, 0, l);
    set_byte(&u->status, pass_chek, true);
    server_say_hello(server, u);
    return 1;
  }
  server_say(401, server, u, "00:badPassword try again\n");
  free(u->msg);
  u->msg = NULL;
  return 0;
}


/// return 0 if not valid
static size_t test_begenig(const char* s) {
  const size_t l = strlen(s);
  if (l < 3) {
    return 0;
  }
  int i = 0;
  while (i < LEN_OF_CODE) {
    if (!isdigit(s[i]))
      return 0;
    i++;
  }
  return s[i] == ':' ? l : 0;
}

static int test_cmd(server_data* server, const t_user* ref) {
  const size_t l = test_begenig(ref->msg);
  if (l == 0) {
    return 1;
  }
  const int code = atoi(ref->msg);
  printf("code = %d\n", code);
  (void)server;
  return 0;
}

static int got_password(server_data* server, t_user* user) {
  if (read_byte(user->status, pass_chek))
    return 0;
  return !ask_password(server, user);
}

#include "fileProssess.h"

static void print_user(t_user* u) {
  printf("fd[%d] id[%u] status[%d]\n", u->fd, u->id, u->status);
}

static void test_user_list(server_data* server) {
  for (size_t i = 0; i < 10; i++) {
    print_user(server->userData.users + i);
  }
  printf("-------------------------\n");
}

int dispatch(server_data* server) {
  t_user* ref = server->userData.users + server->userData.read;
  if (strcmp("exit", ref->msg) == 0) { //! only for dev
    free(ref->msg);
    ref->msg = NULL;
    return 1;
  }
  if (strcmp("list", ref->msg) == 0) { //! only for dev
    test_user_list(server);
    free(ref->msg);
    ref->msg = NULL;
    return 0;
  }
  if (strncmp("GET / HTTP/1.1\r\n", ref->msg, strlen("GET / HTTP/1.1\r\n")) == 0) {
    t_file* f = open_file("doc/test.html");
    send_to_user(server, ref, "HTTP/1.1 200 OK\r\n");
    send_to_user(server, ref, "Content-Type: text/html\r\n");
    send_to_user(server, ref, "Connection: close\r\n\r\n");
    for (size_t i = 0; i < f->arraySize - 1; i++) {
      send_to_user(server, ref, read_line(f, i));
      send_to_user(server, ref, "\r\n");
    }
    close_file(&f);
    set_byte(&ref->status, force_dc, true);
    return 0;
  }
  test_cmd(server, ref);
  if (*ref->msg == 0) {
    return 0;
  }
  if (got_password(server, ref)) {
    return 0;
  }
  t_msg m = {
    txt,
    0,
    strlen(ref->msg),
    ref->msg,
    NULL
  };
  ref->msg = NULL;
  add_msg(&server->def, m);
  return 0;
}