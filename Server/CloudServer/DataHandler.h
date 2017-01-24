#include <iostream>
#include <time.h>
#include <sys/time.h>
#include "DataUnit.h"
using namespace std;

class DataHandler{
public:
	DataHandler(){

	}

	void setRecvData(int d, int i){
		recvData[i] = d;
	}

	void printRecvData(){
		gettimeofday(&tv,0);  
		unsigned long timenow = tv.tv_sec * 1000 + tv.tv_usec / 1000;
		cout<<endl<<"timenow:-----------------"<<timenow<<endl;
		cout<<"10 R:";
		for(int i=0;i<10;++i)
			cout<<recvData[i]<<" ";

		cout<<endl<<"time:"<<recvData[46]<<" "<<"1st jy901:";
		for(int i=10;i<19;++i)
			cout<<recvData[i]<<" ";

		cout<<endl<<"time:"<<(recvData[46]+recvData[47])<<" "<<"2nd jy901:";
		for(int i=19;i<28;++i)
			cout<<recvData[i]<<" ";

		cout<<endl<<"time:"<<(recvData[46]+recvData[48])<<" "<<"3rd jy901:";
		for(int i=28;i<37;++i)
			cout<<recvData[i]<<" ";

		cout<<endl<<"time:"<<(recvData[46]+recvData[49])<<" "<<"4th jy901:";
		for(int i=37;i<46;++i)
			cout<<recvData[i]<<" ";

		cout<<endl;

		for(int i=0;i<4;++i){
			cout<<"construct dataunit-"<<i<<":"<<endl;
			//cout<<(*())
			DataUnit d = DataUnit(recvData+(i*9+10));
		}

		// data[i/3] = ((float)d)*16/32768;
		// data[i/3] = (float)d*2000/32768;
		// data[i/3] = (float)d*180/32768;
	}



	~DataHandler(){

	}

private:
	int recvData[50];
	struct timeval tv;
	unsigned long lasttime;
	int lastZAngle;
	static float v;
};

float DataHandler::v = 0.0f;