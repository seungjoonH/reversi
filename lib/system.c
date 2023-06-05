#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enums.h"
#include "system.h"

char *errMsg[ERR_LEN] = {
	"Wrong number of arguments\nUsage:\n./reversi -server [Port]\n./reversi -client [Ip addr] [Port]"
};

void terminate(ErrorType type) {
	fprintf(stderr, "%s\n", errMsg[type]);
	exit(-1);
}

Role getRole() {
	Role role;
	if (!strcmp(iptvec[1], "-client")) role = CLIENT;
	else if (!strcmp(iptvec[1], "-server")) role = SERVER;

	if (iptlen != (role < 0) + 3) terminate(WRN_PM);
	
	return role;
}

char *getIp() {
	char *ip = (char *) malloc(16 * sizeof(char));
	switch (getRole()) {
		case CLIENT: ip = iptvec[2]; break;
		default: ip = NULL; break;
	}
	return ip;
}

int getPort() {
	switch (getRole()) {
		case CLIENT: return atoi(iptvec[3]);
		case SERVER: return atoi(iptvec[2]);
	}
}
