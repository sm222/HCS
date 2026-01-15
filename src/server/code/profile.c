# include "server.h"
# include "fileProssess.h"

# define DEF_PROFILE_FILE "data/.user"


/*
static int32_t look_for_user_profile(server_data* server) {
  int32_t file = test_file_perms(DEF_PROFILE_FILE);
  return file;
}
*/

/*
int init_profile(server_data* server) {
  if (!server) {
    return 1;
  }
  bzero(server->profile, sizeof(t_user) * MAX_USER);
  if (look_for_user_profile(server) == 1) {
    return 2;
  }
  return 0;
}
*/