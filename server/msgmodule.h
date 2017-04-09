#ifndef __MSGMOD_H__
#define __MSGMOD_H__

#include "libs.h"

char *msg_not_implemented();

char *msg_not_found();

char *msg_ok(char *contents, char *connection, char* filetype,int len);

#endif
