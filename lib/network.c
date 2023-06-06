#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <linux/socket.h>

#include "ui.h"
#include "network.h"
#include "system.h"

void setArgs() {
	role = getRole();
	ip = getIp();
	port = getPort();
}

void rungame() {
	setArgs();

	switch (role) {
		case CLIENT: runClient(); break;
		case SERVER: runServer(); break;
		default: break;
	}
}

void runClient() {
	int c;
	int connFd = connectToServer(ip, port);

	initUI();

	while (1) {
		char received[256];
		int bytes = receiveMessage(connFd, received, sizeof(received));
		if (bytes > 0) data = decodeData(received);

		redraw();

		c = execute();
		if (c == 'q') { disposeUI(); break; }

		redraw();

		char *encoded = encodeData(data);
		sendMessage(connFd, encoded);
	}

	shutdown(connFd, SHUT_RDWR);
}

void runServer() {
	int c;
	int connFd = listenAtPort(port);

	initUI();

	while (1) {
		char *encoded = encodeData(data);
		sendMessage(connFd, encoded);

		char received[256];
		int bytes = receiveMessage(connFd, received, sizeof(received));
		if (bytes > 0) data = decodeData(received);
		
		redraw();

		c = execute();
		if (c == 'q') { disposeUI(); break; }
		
		redraw();
	}

	shutdown(connFd, SHUT_RDWR);
}

char *encodeData(int **data) {
	char *encoded = (char *) malloc(100 * sizeof(char));
	encoded[0] = '\0';

	for (int i = 0; i < gridR; i++) {
		char row[30];
		row[0] = '\0';

		for (int j = 0; j < gridC; j++) {
			char e[2];
			sprintf(e, "%d", data[i][j]);
			strcat(row, e);
			strcat(row, ",");
		}

		strcat(encoded, row);
		strcat(encoded, ";");
	}

	return encoded;
}

int** decodeData(char* str) {
	int** decoded = (int**)malloc(gridR * sizeof(int*));
	for (int i = 0; i < gridR; i++) {
		decoded[i] = (int*)malloc(gridC * sizeof(int));
	}

	char* token = str;
	int rowIndex = 0;
	int colIndex = 0;
	while (*token != '\0') {
		if (*token == ',' || *token == ';') {
			token++; continue;
		}

		if (colIndex >= gridC) {
			rowIndex++;
			colIndex = 0;
		}

		decoded[rowIndex][colIndex] = atoi(token);
		colIndex++;

		token = strchr(token, ',');
		if (token == NULL) break;
	}

	return decoded;
}


int listenAtPort(int portnum) {
	int sock_fd = socket(AF_INET /*IPv4*/, SOCK_STREAM /*TCP*/, 0 /*IP*/);
	if (sock_fd == 0)  {
		perror("socket failed : ");
		exit(EXIT_FAILURE);
	}
	int opt = 2;
	if (setsockopt(sock_fd, SOL_TCP, TCP_NODELAY, &opt, sizeof(opt)) != 0) {
		fprintf(stderr, "fail at setsockopt\n");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in address;
	bzero(&address, sizeof(address)); 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY /* localhost */;
	address.sin_port = htons(portnum);

	if (bind(sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed: ");
		exit(EXIT_FAILURE);
	}

	if (listen(sock_fd, 16 /* the size of waiting queue*/) < 0) {
		perror("listen failed : ");
		exit(EXIT_FAILURE);
	}

	int addrlen = sizeof(address);
	int conn_fd = accept(sock_fd, (struct sockaddr *) &address, (socklen_t*)&addrlen);
	if (conn_fd < 0) {
		perror("accept failed: ");
		exit(EXIT_FAILURE);
	}
	return conn_fd;
}

int connectToServer(const char* ip, int port) {
	int sock_fd;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd <= 0) {
		perror("socket failed : ");
		exit(EXIT_FAILURE);
	}
	int opt = 2;
	if (setsockopt(sock_fd, SOL_TCP, TCP_NODELAY, &opt, sizeof(opt)) != 0) {
		fprintf(stderr, "fail at setsockopt\n");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	if (inet_pton(AF_INET, ip, &address.sin_addr) <= 0) {
		perror("inet_pton failed : ");
		exit(EXIT_FAILURE);
	}

	if (connect(sock_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
		perror("connect failed : ");
		exit(EXIT_FAILURE);
	}
	return sock_fd;
}

int sendMessage(int connFd, const char* message) {
	int len = strlen(message);
	int sent = send(connFd, message, len, 0);
	return sent;
}

int receiveMessage(int connFd, char* buf, int bufSize) {
	int received = recv(connFd, buf, bufSize - 1, 0);
	if (received > 0) buf[received] = '\0';
	return received;
}

