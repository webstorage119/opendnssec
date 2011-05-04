#ifndef _KEYSTATE_UPDATE_KEYZONES_CMD_H_
#define _KEYSTATE_UPDATE_KEYZONES_CMD_H_

#include "daemon/engine.h"

void help_update_keyzones_cmd(int sockfd);

int handled_update_keyzones_cmd(int sockfd, engine_type* engine, 
                                 const char *cmd, ssize_t n);

#endif
