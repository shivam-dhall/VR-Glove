#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <iostream>
#include <time.h>
#include <sys/time.h>
#include "DataUnit.h"
#include "DataList.h"
using namespace std;

#define g 9.8
#define FILTER_WIDTH 0.0009

class DataHandler{
public:
	DataHandler(){
		lastDataUnit = DataUnit();
		dataList = DataList::getInstance();
	}

	void setRecvData(int d, int i){
		recvData[i] = d;
	}

	void handleRecvData(){
		int time[4];
		time[0] = recvData[46];
		time[1] = recvData[46]+recvData[47];
		time[2] = recvData[46]+recvData[48];
		time[3] = recvData[46]+recvData[49];
		for(int i=0;i<4;++i){
			//cout<<"construct dataunit-"<<i<<":"<<endl;
			dataUnit[i] = DataUnit(recvData+(i*9+10),time[i]);
			//cout<<"datahandler time:"<<dataUnit[i].getTime()<<endl;
			dataList->addDataUnit(recvData+(i*9+10),time[i]);
		}

		float a = (lastDataUnit.getAcc().getX()+dataUnit[0].getAcc().getX())/2;
		float t = dataUnit[0].getTime() - lastDataUnit.getTime();
		cout<<"cal_a0:"<<a<<endl;
		if(a>=FILTER_WIDTH||a<=-FILTER_WIDTH){
			float ttt = a*g*t/1000;
			v += (a*g*t/1000);
			cout<<"plus:"<<ttt<<endl;
		}

		for(int i=1;i<4;++i){
			a = (dataUnit[i-1].getAcc().getX()+dataUnit[i].getAcc().getX())/2;
			cout<<"cal_a"<<i<<":"<<a<<endl;
			t = dataUnit[i].getTime() - dataUnit[i-1].getTime();
			if(a>=FILTER_WIDTH||a<=-FILTER_WIDTH){
				v += (a*g*t/1000);
				float ttt = a*g*t/1000;
				cout<<"plus:"<<ttt<<endl;
			}
			
		}

		cout<<"-----------------v:"<<v<<endl;

		// data[i/3] = ((float)d)*16/32768;
		// data[i/3] = (float)d*2000/32768;
		// data[i/3] = (float)d*180/32768;
	}

	void printRecvData(){
		gettimeofday(&tv,0);  
		unsigned long timenow = tv.tv_sec * 1000 + tv.tv_usec / 1000;
		cout<<endl<<"timenow:-----------------"<<timenow<<endl;
		// cout<<"10 R:";
		// for(int i=0;i<10;++i)
		// 	cout<<recvData[i]<<" ";

		
		// cout<<endl<<"time:"<<recvData[46]<<" "<<"1st jy901:";
		// for(int i=10;i<19;++i)
		// 	cout<<recvData[i]<<" ";

		// cout<<endl<<"time:"<<(recvData[46]+recvData[47])<<" "<<"2nd jy901:";
		// for(int i=19;i<28;++i)
		// 	cout<<recvData[i]<<" ";

		// cout<<endl<<"time:"<<(recvData[46]+recvData[48])<<" "<<"3rd jy901:";
		// for(int i=28;i<37;++i)
		// 	cout<<recvData[i]<<" ";

		// cout<<endl<<"time:"<<(recvData[46]+recvData[49])<<" "<<"4th jy901:";
		// for(int i=37;i<46;++i)
		// 	cout<<recvData[i]<<" ";

		// cout<<endl;

	}

	~DataHandler(){
		if(dataList != NULL)
			delete dataList;
	}

private:
	int recvData[50];
	struct timeval tv;
	unsigned long lasttime;
	int lastZAngle;
	static float v;
	DataList *dataList;
	DataUnit lastDataUnit;
	DataUnit dataUnit[4];
};

float DataHandler::v = 0.0f;

#endif