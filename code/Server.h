#pragma once
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include <unistd.h>

using namespace std;



class Server {
private:
	struct sockaddr_in self_sockaddr;
	socklen_t clientAddrLen = sizeof(self_sockaddr);
	int selfSocket = 0;  //welcome socket
	enum status { FAILURE, SUCCESS };
public:
	Server();
	int start();

};
void cleanHandle(int sig);// server uses to clean the child process



class User {
private:
	char clientBuff[8196]; // each one there are 1024*8 data messages at maximum.
	char targetBuff[8196];
	int targetSocket = 0;  //use this Socket to connect remote target server.
	int clientSocket = 0;  //use this Socket to connect client.
	struct sockaddr_in client_sockaddr;
	socklen_t client_size = sizeof(client_sockaddr);
public:
	User();
	int acceptClient(int welcomesock);
	void closeClientSock();
	int readMessages();
	void cleanBuffer();
	int sendMessage();
	int processHTTPMessage();
};