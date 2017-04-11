#ifndef __MSGMOD_H__
#define __MSGMOD_H__

#include "libs.h"

char *msg_not_implemented();

char *msg_not_found();

char *msg_ok(char *contents, char *connection, char* filetype,int len);

char *msg_deleted_ok(char *con);

char *msg_not_deleted_ok(char *con);


char *msg_bad_request(char *con);

#endif
