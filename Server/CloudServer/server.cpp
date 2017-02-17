#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>
#include <pthread.h>
//#include <syswait.h>
#include "MySocket.h"
#include <fstream>
using namespace std;


//#define MYPORT  8080
//#define QUEUE   20
//#define BUFFER_SIZE 4096

//void myStrcopy(char *&dst, char* src, int start, int size){
//	int cnt = 0;
//	while (cnt < start){
//		++dst;
//		++cnt;
//	}
//	cnt = 0;
//	while (cnt<size){
//		*(dst++) = *(src++);
//		++cnt;
//	}
//}
//
//void SendData(char *data,int size, int type,int s){
//	char a[5];
//	a[0] = (char)((size >> 24) & 0xFF);
//	a[1] = (char)((size >> 16) & 0xFF);
//	a[2] = (char)((size >> 8) & 0xFF);
//	a[3] = (char)(size & 0xFF);
//	a[4] = (char)(type & 0xFF);
//	int ret = size;
//	send(s, a, 5, 0);
//	send(s, data, size, 0);
//}
MySocket *sock;
ofstream out("data.txt");

void *threadFunc1(void *arg){
	while (1){		
		sock->BeginWork(out);
		usleep(10000);
		
	}
}


int main(int argc, char *argv[])
{
	bool isUnityConnect = false;
	cout<<"argc:"<<argc<<endl;
	for(int i=0;i<argc;++i)
		printf("option: %s\n", argv[i]);
	if(argv[1][0]=='1'){
		cout<<"true"<<endl;
		isUnityConnect;
	}

	sock = MySocket::getInstance();
	sock->OpenPort();
	sock->WaitClient();
	if(isUnityConnect)
		sock->WaitClient();
		
	pthread_t tid1;
	int err;
	err = pthread_create(&tid1, NULL, threadFunc1, NULL);//创建线程
	if (err != 0)
	{
		printf("pthread_create error:%s\n", strerror(err));
		exit(-1);
	}

	while (1){
		cout << "*********** " << endl;
		usleep(1000000);
	}

	//char a[23456];
	//cout << "***" << endl;
	//a[0] = 'z';
	//for (int i = 1; i < 23455; ++i)
	//	a[i] = 'a';
	//a[23455] = 'y';
	//cout << "-----" << endl;
	//usleep(200000);
	//while (1){
	//sock->SendData(a, 23456, 1);
	//usleep(100000);
	//}
	//char ttt1[100];
	//ttt1[0] = 'a';
	//char ttt2[5];
	//for (int i = 0; i < 5; ++i)
	//	ttt2[i] = 'b';
	//char * index = ttt1;
	//myStrcopy(index, ttt2, 1, 5);
	//cout << index << endl;

 //   ///定义sockfd
 //   int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

 //   ///定义sockaddr_in
 //   struct sockaddr_in server_sockaddr;
 //   server_sockaddr.sin_family = AF_INET;
 //   server_sockaddr.sin_port = htons(MYPORT);
 //   server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

 //   ///bind，成功返回0，出错返回-1
 //   if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
 //   {
 //       perror("bind");
 //       exit(1);
 //   }

 //   ///listen，成功返回0，出错返回-1
 //   if(listen(server_sockfd,QUEUE) == -1)
 //   {
 //       perror("listen");
 //       exit(1);
 //   }

 //   ///客户端套接字
 //   char buffer[BUFFER_SIZE];
 //   struct sockaddr_in client_addr;
 //   socklen_t length = sizeof(client_addr);

 //   cout<<"before connect"<<endl;

 //   ///成功返回非负描述字，出错返回-1
 //   int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
 //   if(conn<0)
 //   {
 //       perror("connect");
 //       exit(1);
 //   }

 //   cout<<"connect success"<<endl;

	//char sendBuf[4096];
	//for (int i = 0; i < 4096; ++i)
	//	sendBuf[i] = 'a';

	//int cnt = 0;

	////char a[4101];
	////int size = 4096;
	////a[0] = (char)((size >> 24) & 0xFF);
	////a[1] = (char)((size >> 26) & 0xFF);
	////a[2] = (char)((size >> 8) & 0xFF);
	////a[3] = (char)(size & 0xFF);
	////a[4] = (char)(1 & 0xFF); 

	////for (int i = 5; i < 4101; ++i)
	////	a[i] = 'a';

	//char a[23456];
	//a[0] = 'a';
	//a[23455] = 'z';
	//for (int i = 0; i < 23455;++i)
	//	a[i] = 'b';

 //   //while(1)
 //   {
	//	//send(conn, a,4101 , 0);

	//	usleep(100000);

 //       //memset(buffer,0,sizeof(buffer));
 //       //int len = recv(conn, buffer, sizeof(buffer),0);
 //       //if(strcmp(buffer,"exit\n")==0)
 //           //break;
 //       //cout<<buffer<<endl;
 //       //fputs(buffer, stdout);
 //       //send(conn, buffer, len, 0);
 //   }
 //   close(conn);
 //   close(server_sockfd);
    return 0;
}