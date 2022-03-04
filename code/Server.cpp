#include"Server.h"



void cleanHandle(int sig){
	cout << "child is quit" << endl;
	pid_t id;
	while ((id = waitpid(-1, NULL, WNOHANG))>0)
		cout << "Clean child " << id << " success " << endl;
}



//以后要更改这个程序，不能把容易出错的代码放在构造函数和构析函数中
Server::Server() {
	signal(SIGCHLD, cleanHandle);//
	cout << "Server Star!" << endl;
	if (!(selfSocket = socket(AF_INET, SOCK_STREAM, 0))) {
		cout << "error, can't distribute a sockfd ! Please stop and retry." << endl;
	}
	else {
		cout << "create socket OK!" << endl;
		self_sockaddr.sin_family = AF_INET;
		self_sockaddr.sin_addr.s_addr = INADDR_ANY;
		self_sockaddr.sin_port = htons(1080);
		if (int bindRes = bind(selfSocket, (struct sockaddr*)&self_sockaddr, sizeof(self_sockaddr)) == EADDRINUSE)
		{
			cout << "Bind ERROR! port has been used!" << endl;
			exit(-1);
		}
		else if (bindRes == 0)
			cout << "Bind SUCCESS!" << endl;
		if (listen(selfSocket, 20) == -1) {
			cout << "Listen error!" << endl;
		}
		else
			cout << "listen success!" << endl;
	}
}



int Server::start(){
		while (true) {
			User user;// COW技术会保证父进程重复创建的时候也会有一份之前的副本在子进程中
			cout << "Wait for some one connect" << endl;
			user.clientSocket = accept(selfSocket, (struct sockaddr*)&user.client_sockaddr, &clientAddrLen);
			if (user.clientSocket == -1) {
				cout << "accept error!" << endl;
				exit(-1);
			}
				cout << "some one connect" << endl;
			if (int pid = fork()) { // 父进程
				close(user.clientSocket);
				cout << "father release the subsocket right" << endl;
				continue;
				}
			else if (pid == 0) {//子进程
				while (true) {
					cout << "Wait message from client!" << endl;
					//同步模式下的socket
					if(read(user.clientSocket, user.clientBuff, sizeof(user.clientBuff)))
						cout << user.clientBuff << endl;
					else {
						cout << "Client close";
						exit(1);
					}
					memset(user.clientBuff, 0, sizeof(user.clientBuff));
				}
			} //else if

			else {
				cout << "fork error ,someting bad happened!" << endl;
			}
		}
	}




User::User() {
	memset(targetBuff, 0, sizeof(targetBuff));
	memset(clientBuff, 0, sizeof(clientBuff));
	cout << "initialize buffer OK!" << endl;
}














//the following function use in the future Socket proxy.
/*
int Server::handshake() {
	int subSocket = 0;
	cout << "I will accept some one !" << endl;
	subSocket = accept(selfSocket, (struct sockaddr*)&client_sockaddr, &clientAddrLen);
	cout << "some one connect!" << endl;
	read(subSocket, clientBuff, sizeof(clientBuff));
	return 1;
}
*/
