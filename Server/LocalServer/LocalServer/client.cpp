#include <winsock2.h>  
#include <stdio.h>  
#pragma comment(lib, "ws2_32.lib")  

#include <iostream>
using namespace std;


int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1, 1);

	WSAStartup(wVersionRequested, &wsaData);

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("101.200.45.113");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8080);
	int ret = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	char recvBuf[100] = { 0 };
	cout << "connect" << endl;
	//recv(sockClient, recvBuf, 100, 0);
	//printf("%s\n", recvBuf);
	while (1){
		send(sockClient, "hello world", strlen("hello world") + 1, 0);
	}
	

	closesocket(sockClient);
	WSACleanup();

	return 0;
}