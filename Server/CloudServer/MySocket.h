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
using namespace std;

#define BUFFER_SIZE 4096
#define PORT  8080
#define QUEUE   20

struct sockaddr_in client_addr;
///定义sockaddr_in
struct sockaddr_in server_sockaddr;

class MySocket{
public:

	MySocket(){//:port(PORT),bufferSize(BUFFER_SIZE){
		port = PORT;
		bufferSize = BUFFER_SIZE;
		server_sockfd = -1;
		conn = -1;
	}

	void setBufferSize(int size){
		bufferSize = size;
	}

	static MySocket *getInstance(){
		if(mySocket == NULL){
			mySocket = new MySocket();
		}
		return mySocket;
	}

	void OpenPort(int port = PORT){
		buffer = new char[bufferSize];
		memset(buffer,0,sizeof(buffer));
		///定义sockfd
	    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);


	    server_sockaddr.sin_family = AF_INET;
	    server_sockaddr.sin_port = htons(PORT);
	    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	    ///bind，成功返回0，出错返回-1
	    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
	    {
	        perror("bind");
	        exit(1);
	    }

	    ///listen，成功返回0，出错返回-1
	    if(listen(server_sockfd,QUEUE) == -1)
	    {
	        perror("listen");
	        exit(1);
	    }

	    ///客户端套接字
	    length = sizeof(client_addr);

	    cout<<"before connect"<<endl;

	    ///成功返回非负描述字，出错返回-1
	    

	}

	bool WaitClient(){
		conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
		if(conn<0)
	    {
	        perror("connect error");
	        exit(1);
	    }

	    cout<<"connect success"<<endl;
		return true;
	}


	void SendData(char* data,int size, int type){
		if(conn == -1){
			cout<<"not connect"<<endl;
			return;
		}

		char* head = buffer;

		buffer[0] = (char)((size >> 24) & 0xFF);
		buffer[1] = (char)((size >> 16) & 0xFF);
		buffer[2] = (char)((size >> 8) & 0xFF);
		buffer[3] = (char)(size & 0xFF);
		buffer[4] = (char)(type & 0xFF);
		char* index = data;
		if(size<=bufferSize-5){
			myStrcopy(buffer,index,5,size);
			send(conn,head,size+5,0);
			return;
		}
		else{
			myStrcopy(buffer,index,5,bufferSize-5);
			send(conn,head,bufferSize,0);
			int ret = size - (bufferSize-5);
			usleep(10000);
			while(ret>0){
				int sendSize = (ret<bufferSize?ret:bufferSize);
				send(conn, index, sendSize, 0);
				ret -= sendSize;
				index +=sendSize;
			}
		}
		
	}


	//copy src's [0,size) to dst[start,start+size)
	void myStrcopy(char *dst, char* &src, int start, int size){
		int cnt = 0;
		while (cnt < start){
			++dst;
			++cnt;
		}
		cnt = 0;
		while (cnt<size){
			*(dst++) = *(src++);
			++cnt;
		}
	}

	~MySocket(){
		if(conn!=-1)
			close(conn);
		if(server_sockfd!=-1)
			close(server_sockfd);
		if(buffer!=NULL)
			delete []buffer;

	}

private:
	int bufferSize;
	int port;
	char* buffer;
	int conn;
	int server_sockfd;
	static MySocket *mySocket;
	socklen_t length;
};

MySocket* MySocket::mySocket = NULL;