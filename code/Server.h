#pragma once
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdlib.h>
#include <unistd.h>
class Server {
private:
	struct sockaddr_in self_sockaddr;
	struct sockaddr_in client_sockaddr;
	socklen_t clientAddrLen = sizeof(self_sockaddr);
	int selfSocket = 0;
	int targetSocket = 0; //use this Socket to connect remote target server.
	int clientSocket = 0; //use this Socket to connect client.
	char clientBuff[1024*8] = {0}; // each time there are 20 data messages at maximum.
	char targetBuff[1024*8] = {0};

	enum status { FAILURE, SUCCESS };
public:
	int handshake();
	Server();
};