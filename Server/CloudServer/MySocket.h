#ifndef MY_SOCKET_H
#define MY_SOCKET_H

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
#include <sys/time.h>  
  
#include "DataHandler.h"

using namespace std;

#define BUFFER_SIZE 4096
#define PORT  8080
#define QUEUE   20

struct sockaddr_in client_addr;
///定义sockaddr_in
struct sockaddr_in server_sockaddr;

struct sockaddr_in client_addr1;
///定义sockaddr_in
struct sockaddr_in server_sockaddr1;

class MySocket{
public:

	MySocket(){//:port(PORT),bufferSize(BUFFER_SIZE){
		port = PORT;
		bufferSize = BUFFER_SIZE;
		bufferSize1 = BUFFER_SIZE;
		server_sockfd = -1;
		connUnity = -1;
		cnt = 0;
		// char i1 = -1;
		// char i2 = -1;
		// short s = (short)((i1<<8)|(i2&0x00FF));
		// int d = s;
		// cout<<s<<" "<<d<<" "<<hex<<s<<" "<<d<<endl;
		//lasttime = 0;
		dataHandler = DataHandler();
	}

	void setBufferSize(int size){
		bufferSize = size;
		bufferSize1 = size;
	}

	static MySocket *getInstance(){

		if(mySocket == NULL){
			mySocket = new MySocket();
		}
		return mySocket;
	}

	void Print(){
		cout<<"data:";
		for(int i=0;i<22;++i){
			cout<<data[i]<<",";
		}
		cout<<endl;
	}


	void BeginWork(ofstream& out){
		cout << "-----------" <<(++cnt)<< endl;
		if(connArduino==-1)
			return;
		///int len = _ReceiveData(connArduino);
		memset(receiveData,0,sizeof(receiveData));
		int len = recv(connArduino, receiveData, bufferSize1,0);
		//cout<<"receive:"<<len<<"data from arduino,"<<endl;
		//for(int i=0;i<153;++i)
			//cout<<i<<":"<<(int)receiveData[i]<<" ";
		//cout<<endl;
		if(len == 153){
			//int temp[50];
			for(int i=0; i<153; i = i+3){
				if(receiveData[i] == receiveData[i+2])
					receiveData[i] -= receiveData[i+2];
				if(receiveData[i+1] == receiveData[i+2])
					receiveData[i+1] -= receiveData[i+2];
				//cout<<i<<":"<<(int)receiveData[i]<<" "<<(int)receiveData[i+1]<<"#";

				int d;
				if(i/3<=9){
					unsigned short dd = (unsigned short)((receiveData[i]<<8)|(receiveData[i+1]&0x00FF));
					dataHandler.setRecvData(dd,i/3);
					//temp[i/3] = dd;
				}
				else if(i/3>=10&&i/3<=45){
					short dd = (short)((receiveData[i]<<8)|(receiveData[i+1]&0x00FF));
					dataHandler.setRecvData(dd,i/3);
					//temp[i/3] = dd;
				}
				else if(i/3==46){
					unsigned int dd = (unsigned int)(((receiveData[i]&0xFF)<<24)|
								 ((receiveData[i+1]&0xFF)<<16)|
								 ((receiveData[i+3]&0xFF)<<8)|
								  (receiveData[i+4]&0xFF));
					dataHandler.setRecvData(dd,i/3);
					//temp[46] = dd;
				}
				else if(i/3>=48&&i/3<=50){
					unsigned short dd = (unsigned short)((receiveData[i]<<8)|(receiveData[i+1]&0x00FF));
					dataHandler.setRecvData(dd,i/3-1);
					//temp[i/3-1] = dd;
				}

			}

			dataHandler.printRecvData();
			dataHandler.handleRecvData();

			if(cnt==100)
				dataHandler.closeFile(out);
			else if(cnt<100)
				dataHandler.recordData(out);

			// Print();
			// gettimeofday(&tv,0);  
			// unsigned long timenow = tv.tv_sec * 1000 + tv.tv_usec / 1000;
			// if(lasttime == 0){
			// 	lasttime = timenow;
			// 	lastZAngle = data[21];
			// }
			// else{
			// 	int interval = timenow - lasttime;
			// 	lasttime = timenow;

			// 	for(int i=16;i<19;++i)
			// 		data[i] *= interval;


			// 	cout<<"nowangle:"<<data[21]<<" lastZAngle:"<<lastZAngle<<" minus:";
			// 	int temp_angle = data[21];
			// 	data[21]-=lastZAngle;
			// 	lastZAngle = temp_angle;
			// 	cout<<data[21]<<endl;

			// 	if(data[21]<1&&data[21]>-1)
			// 		data[21] = -361;

			// 	int temp[22];
			// 	for(int i=0;i<22;++i)
			// 		temp[i] = (int)data[i];
			// 	_SendData(connUnity,temp,22,1);
			// }
			
		}
		//cout<<endl;

		
	}

	void OpenPort(int port = PORT){
		buffer = new char[bufferSize];
		memset(buffer,0,sizeof(buffer));
		receiveData = new char[bufferSize1];
		memset(receiveData,0,sizeof(receiveData));
		///定义sockfd
	    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

	    int on = 1;
		int ret = setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

	    server_sockaddr.sin_family = AF_INET;
	    server_sockaddr.sin_port = htons(PORT);
	    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	    ///bind，成功返回0，出错返回-1
	    if((ret = bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr)))==-1)
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
		int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
		if(conn<0)
	    {
	        perror("connect error");
	        exit(1);
	    }

	    cout<<"connect success"<<endl;
	    char a[2];
	    int len = recv(conn, a, 2,0);
	    cout<<"len:"<<len<<endl;
	    if(a[0]=='1'){
	    	connArduino = conn;
	    	cout << "arduino client connect" << endl;
	    }
	    else if(a[0]=='2'){
	    	connUnity = conn;
	    	cout << "unity connect" << endl;
	    }
		return true;
	}


	~MySocket(){
		if(connUnity!=-1)
			close(connUnity);
		if(connArduino!=-1)
			close(connUnity);
		if(server_sockfd!=-1)
			close(server_sockfd);

		if(buffer!=NULL)
			delete []buffer;
		if(receiveData!=NULL)
			delete []receiveData;

	}

private:

	
	int _ReceiveData(int conn){
		memset(receiveData,0,sizeof(receiveData));
		int l = 0;
		char * index = receiveData;
		while(l<5){
			int len = recv(conn,index,bufferSize1,0);
			l+=len;
			index +=len;
		}
		int packSize = (int)((receiveData[0] & 0xFF << 24)
                               | ((receiveData[1] & 0xFF) << 16)
                               | ((receiveData[2] & 0xFF) << 8)
                               | ((receiveData[3] & 0xFF)));
		cout<<"packSize:"<<packSize<<endl;
		while(l-4<packSize){
			int rest = packSize-l+4;
			int len = recv(conn,index,(rest<bufferSize1?rest:bufferSize1),0);
			l+=len;
			index +=len;
		}

		return l;

	}

	void _SendData(int conn,int *arr,int size,int type){
		if(conn == -1){
			cout<<"not connect"<<endl;
			return;
		}
		memset(buffer,0,sizeof(buffer));

		size *= 4;

		buffer[0] = (char)((size >> 24) & 0xFF);
		buffer[1] = (char)((size >> 16) & 0xFF);
		buffer[2] = (char)((size >> 8) & 0xFF);
		buffer[3] = (char)(size & 0xFF);
		buffer[4] = (char)(type & 0xFF);
		for(int i=0;i<size/4;++i)
			cout<<arr[i]<<"--";
		cout<<endl;

		for(int i=0;i<size/4;++i){
			buffer[i*4+5] = (char)((arr[i] >> 24) & 0xFF);
			buffer[i*4+6] = (char)((arr[i] >> 16) & 0xFF);
			buffer[i*4+7] = (char)((arr[i] >> 8) & 0xFF);
			buffer[i*4+8] = (char)((arr[i]) & 0xFF);
		}

		char *index = buffer;		
		int ret = size + 5;
		cout<<"send "<<ret<<"data:";
		for(int i=0;i<ret;++i)
			cout<<(int)buffer[i]<<",";
		cout<<endl;
		while(ret>0){
			int sendSize = (ret<bufferSize?ret:bufferSize);
			send(conn, index, sendSize,0);
			ret -= sendSize;
			index += sendSize;
		}
	}


	void _SendData(int conn,char* data,int size, int type){
		if(conn == -1){
			cout<<"not connect"<<endl;
			return;
		}
		memset(buffer,0,sizeof(buffer));

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

	int bufferSize;
	int port;
	char* buffer;
	int connUnity;
	int server_sockfd;
	static MySocket *mySocket;
	socklen_t length;

	char* receiveData;
	int bufferSize1;
	int connArduino;
	float data[22];

	int cnt;
	// struct timeval tv;
	// unsigned long lasttime;
	// int lastZAngle;

	DataHandler dataHandler;
};

MySocket* MySocket::mySocket = NULL;

#endif