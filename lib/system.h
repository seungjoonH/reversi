#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "enums.h"

int iptlen;
char **iptvec;
char *errMsg[ERR_LEN];

void terminate(ErrorType type);
Role getRole();
char *getIp();
int getPort();

#endif
