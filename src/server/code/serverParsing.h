#ifndef  SERVER_PARSING_H
# define SERVER_PARSING_H

# include "user.h"
# include "server.h"

int dispatch(server_data* server);
void server_say(const size_t bSize, server_data* server, t_user* u, const char* msg, ...);

#endif