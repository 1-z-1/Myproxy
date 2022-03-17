#include"Server.h"


//signal process function(SIGCHILD)
void cleanHandle(int sig){
	cout << "child is quit" << endl;
	pid_t id;
	while ((id = waitpid(-1, NULL, WNOHANG))>0)
		cout << "Clean child " << id << " success " << endl;
}




Server::Server() {
	// while switch back to the user mode it will check the signal SIGCHILD
	signal(SIGCHLD, cleanHandle);
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
			user.acceptClient(selfSocket);
			if (int pid = fork()) { // 父进程
				user.closeClientSock();
				cout << "father release the subsocket right" << endl;
				continue;
				}
			else if (pid == 0) {//子进程
				close(selfSocket); //warrent there is only one listen sock
				while (true) {
					user.readMessages();
					user.processHTTPMessage();
					user.sendMessage();
				}
			}
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

void User::closeClientSock() {
	close(clientSocket);
}

int User::acceptClient(int welcomesock) {
	cout << "Wait for some one connect" << endl;
	clientSocket = accept(welcomesock, (struct sockaddr *) & client_sockaddr, &client_size);
	if (clientSocket == -1) {
		cout << "Bad accept!" << endl;
		exit(-1);
	}
	else {
		cout << "accept success" << endl;
	}
}

int User::readMessages() {
	cout << "Waiting messages" << endl;
	if (int flag = recv(clientSocket, clientBuff, sizeof(clientBuff),0))
		cout << "receive "<<flag<<" bytes message" << endl;
	else if(flag<=0) {
		cout <<"receive error" << endl;
		exit(-1);
	}
}
void User::cleanBuffer(){
	memset(clientBuff, 0, sizeof(clientBuff));
}

int User::sendMessage() {
	cout << clientBuff << endl;
	cout << "send success" << endl;
}

int User::processHTTPMessage() {
	string Message = this->clientBuff;
	int startPosi = Message.find("Host")+6;   //去除"Host: "
	int stopPost = Message.find('\n', startPosi);
	string addr = Message.substr(startPosi, stopPost-startPosi); //www.baidu.com:443 
	string host = addr.substr(0, addr.find(':'));
	string port = addr.substr(addr.find(':')+1);
	int n = Message.find("Proxy-Connection");
	Message.replace(n, 16, "Connection");
	memset(clientBuff, 0, sizeof(clientBuff));
	Message.copy(clientBuff, Message.size(), 0);
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
