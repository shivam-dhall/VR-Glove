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
//#include <syswait.h>
using namespace std;

#define MYPORT  8080
#define QUEUE   20
#define BUFFER_SIZE 4096

void SendData(char* data, int type,int s){
	char a[5];
	int size = strlen(data);
	a[0] = (char)((size >> 24) & 0xFF);
	a[1] = (char)((size >> 16) & 0xFF);
	a[2] = (char)((size >> 8) & 0xFF);
	a[3] = (char)(size & 0xFF);
	a[4] = (char)(type & 0xFF);
	int ret = size;
	send(s, a, 5, 0);
	send(s, data, strlen(data), 0);
}



int main()
{

	cout << ":"<<char(0) << endl;

    //printf("11111");
    cout<<"22222"<<endl;
    ///定义sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
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
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    cout<<"before connect"<<endl;

    ///成功返回非负描述字，出错返回-1
    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if(conn<0)
    {
        perror("connect");
        exit(1);
    }

    cout<<"end connect"<<endl;

	char sendBuf[4096];
	for (int i = 0; i < 4096; ++i)
		sendBuf[i] = 'a';

	int cnt = 0;

	char a[4101];
	int size = 4096;
	a[0] = (char)((size >> 24) & 0xFF);
	a[1] = (char)((size >> 26) & 0xFF);
	a[2] = (char)((size >> 8) & 0xFF);
	a[3] = (char)(size & 0xFF);
	a[4] = (char)(1 & 0xFF); 
	//cout << ((size>>56) & 0xFF) << endl;

	cout << (int)a[0] << " " << (int)a[1] << " " << (int)a[2] << " " << (int)a[3] << " " << (int)a[4] << endl;

	for (int i = 5; i < 4101; ++i)
		a[i] = 'a';


    while(1)
    {
		//SendData(sendBuf, 1, conn);
		send(conn, a,4101 , 0);//strlen(sendBuf) + 1
		//for (int i = 0; i < 32768; ++i)
			//sendBuf[i] = (char)('a'+(cnt++)%26);
		//cout << sendBuf[32767] << endl;
		//char sendBuf[20] = "abcde";
		//send(conn, makeDat(4096,1), strlen(sendBuf) + 1, 0);

		//SendData(sendBuf, 1,conn);
		usleep(100000);

        //memset(buffer,0,sizeof(buffer));
        //int len = recv(conn, buffer, sizeof(buffer),0);
        //if(strcmp(buffer,"exit\n")==0)
            //break;
        //cout<<buffer<<endl;
        //fputs(buffer, stdout);
        //send(conn, buffer, len, 0);
    }
    close(conn);
    close(server_sockfd);
    return 0;
}