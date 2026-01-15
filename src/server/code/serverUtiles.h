#ifndef  SERVER_UTILES_H
# define SERVER_UTILES_H

# include "../../dup/dataType.h"
# include "server.h"



int send_to_user(server_data* server, t_user* u, const char* msg);
int send_str(server_data* server, size_t i, const char* msg);
int send_str_all(server_data* server, const char* msg, const char* from);

char* join_free(char* s1, const char* s2);


#endif