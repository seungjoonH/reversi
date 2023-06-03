/* NETWORK.C  */

/* PREPROCESSORS */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <linux/socket.h>

/* FUNCTION PROTOTYPES */
int listenAtPort(int portnum);
int connectToServer(const char* ip, int port);
int sendMessage(int conn_fd, const char* message);
int receiveMessage(int conn_fd, char* buffer, int buffer_size);

/* MAIN FUNCTION */
int main(int argc, char* argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Wrong number of arguments\n");
		fprintf(stderr, "Usage: ./network [role] [IP addr] [Port num]\n");
		exit(EXIT_FAILURE);
	}

	const char* role = argv[1];
	const char* ip = argv[2];
	int port = atoi(argv[3]);

	if (!strcmp(role, "server")) {
		int conn_fd = listenAtPort(port);

		while (1) {
			// received message from client
			char received[256];
			int bytes = receiveMessage(conn_fd, received, sizeof(received));
			if (bytes > 0) printf("Client: %s\n", received);

			// message to send
			char message[256];
			printf("To client: ");
			fgets(message, sizeof(message), stdin);
			message[strlen(message) - 1] = '\0';

			if (!strcmp(message, "quit")) break;
			sendMessage(conn_fd, message);
		}
	} 
	else if (!strcmp(role, "client")) {
		int conn_fd = connectToServer(ip, port);

		while (1) {
			// message to send
			char message[256];
			printf("To server: ");
			fgets(message, sizeof(message), stdin);
			message[strlen(message) - 1] = '\0';

			if (!strcmp(message, "quit")) break;
			sendMessage(conn_fd, message);

			// received message from server
			char received[256];
			int bytes = receiveMessage(conn_fd, received, sizeof(received));
			if (bytes > 0) printf("Server: %s\n", received);
		}
	} 
	else {
		fprintf(stderr, "Invalid role\n");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}


/* FUNCTIONS */
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

int sendMessage(int conn_fd, const char* message) {
	int len = strlen(message);
	int bytes_sent = send(conn_fd, message, len, 0);
	return bytes_sent;
}

int receiveMessage(int conn_fd, char* buffer, int buffer_size) {
	int bytes_received = recv(conn_fd, buffer, buffer_size - 1, 0);
	if (bytes_received > 0) buffer[bytes_received] = '\0';

	return bytes_received;
}

