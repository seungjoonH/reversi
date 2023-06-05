#ifndef _NETWORK_H
#define _NETWORK_H

int listenAtPort(int portnum);
int connectToServer(const char *ip, int port);
int sendMessage(int connFd, const char *message);
int receiveMessage(int connFd, char *buf, int bufSize);

#endif
