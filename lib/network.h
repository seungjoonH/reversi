#ifndef _NETWORK_H
#define _NETWORK_H

#include "enums.h"

Role role;
char *ip;
int port;

void setArgs();
void rungame();
void runClient();
void runServer();
char *encodeData(int **data);
int **decodeData(char *str);
int listenAtPort(int portnum);
int connectToServer(const char *ip, int port);
int sendMessage(int connFd, const char *message);
int receiveMessage(int connFd, char *buf, int bufSize);

#endif
