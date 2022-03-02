#include"Server.h"
using namespace std;
Server::Server() {
	cout << "Server Star!" << endl;
	if (!(selfSocket = socket(AF_INET, SOCK_STREAM, 0))) {
		cout << "error, can't distribute a sockfd ! Please stop and retry."<<endl;
	}
	else {
		int res1 = 5,res2 = 5;
		cout << "create success!" << endl;
		self_sockaddr.sin_family = AF_INET;
		self_sockaddr.sin_addr.s_addr = INADDR_ANY;
		self_sockaddr.sin_port = htons(1080);
		res1 = bind(selfSocket, (struct sockaddr*)&self_sockaddr, sizeof(self_sockaddr));
		res2 = listen(selfSocket, 20);

	}
}

//已经可以实现接受了，明天写
int Server::handshake() {
	int subSocket = 0;
	cout << "I will accept some one !" << endl;
	subSocket = accept(selfSocket, (struct sockaddr*)&client_sockaddr, &clientAddrLen);
	cout << "some one connect!" << endl;
	read(subSocket, clientBuff, sizeof(clientBuff));
	for (int i = 0; i < sizeof(clientBuff); i++)
		cout << clientBuff[i];
	write(subSocket, clientBuff, 100);
	return 1;
}