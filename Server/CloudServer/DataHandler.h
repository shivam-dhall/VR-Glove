#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <iostream>
#include <time.h>
#include <sys/time.h>
#include "DataUnit.h"
#include "DataList.h"
#include <fstream>
using namespace std;

#define g 9.8
#define ACC_FILTER_WIDTH 0.0009
#define V_FILTER_WIDTH 0.5


class DataHandler{
public:
	DataHandler(){
		dataUnit[4] = DataUnit();
		dataList = DataList::getInstance();
		//ofstream out;
		//out.open("hello.txt");
	}

	void setRecvData(int d, int i){
		recvData[i] = d;
	}

	void closeFile(ofstream &out){
		if (out.is_open())
			out.close();
	}

	void handleRecvData(ofstream &out,bool isrecord){
		int time[4];
		time[0] = recvData[46];
		time[1] = recvData[46]+recvData[47];
		time[2] = recvData[46]+recvData[48];
		time[3] = recvData[46]+recvData[49];
		for(int i=0;i<4;++i){
			dataUnit[i] = DataUnit(recvData+(i*9+10),time[i]);
		}

		float a;
		float t;

		for(int i=0;i<4;++i){
			
			dataList->addDataUnit(recvData+(i*9+10),time[i]);
			dataList->setTotalVelocity(getVelocity());
			///////ax
			float tempa = dataUnit[i].getAcc().getX() - dataUnit[(i-1+5)%5].getAcc().getX();
			if(tempa>0.105 && tempa<0.14)
				dataUnit[i].getAcc().setAcc(dataUnit[i].getAcc().getX()-tempa,dataUnit[i].getAcc().getY(),dataUnit[i].getAcc().getZ());
			else if(tempa>=0.14){
				if(i<3){
					float _x = (dataUnit[(i-1+5)%5].getAcc().getX()+dataUnit[i+1].getAcc().getX())/2;
					dataUnit[i].getAcc().setAcc(_x,dataUnit[i].getAcc().getY(),dataUnit[i].getAcc().getZ());
				}
				else
					dataUnit[i].getAcc().setAcc(dataUnit[i].getAcc().getX()-tempa,dataUnit[i].getAcc().getY(),dataUnit[i].getAcc().getZ());
			}

			a = (dataUnit[(i-1+5)%5].getAcc().getX()+dataUnit[i].getAcc().getX())/2;
			cout<<"cal_ax"<<i<<":"<<a<<endl;
			t = dataUnit[i].getTime() - dataUnit[(i-1+5)%5].getTime();
			float plus = 0.0f;
			if(a>=ACC_FILTER_WIDTH||a<=-ACC_FILTER_WIDTH){
				velocity[0] += (a*g*100*t/1000);
				plus = a*g*100*t/1000;
				cout<<"plus_x:"<<plus<<"cm/s"<<endl;
			}
			//if(fabs(velocity[0])<dataList->getStaticWidth())
			if(dataList->isStatic())
				velocity[0] = 0.0f;
			if(fabs(velocity[0])>V_FILTER_WIDTH)
				shifting[0] += (velocity[0]*t/1000);
			// if(isrecord&&out.is_open())
			// 	out<<dataUnit[i].getTime()<<"\t"<<dataUnit[i].getAcc().getX()<<"\t"<<velocity[0]<<"\t"<<shifting[0]<<"\t";//"\t"<<plus<<
			dataList.getDataUnitThisLoop(i).setV_X(velocity[0]);
			dataList.getDataUnitThisLoop(i).setS_X(shifting[0]);


			///////ay
			plus = 0.0f;
			a = (dataUnit[(i-1+5)%5].getAcc().getY()+dataUnit[i].getAcc().getY())/2;
			cout<<"cal_ay"<<i<<":"<<a<<endl;
			t = dataUnit[i].getTime() - dataUnit[(i-1+5)%5].getTime();
			if(a>=ACC_FILTER_WIDTH||a<=-ACC_FILTER_WIDTH){
				velocity[1] += (a*g*100*t/1000);
				plus = a*g*100*t/1000;
				cout<<"plus_y:"<<plus<<"cm/s"<<endl;
			}
			//if(fabs(velocity[0])<dataList->getStaticWidth())
			if(dataList->isStatic())
				velocity[1] = 0.0f;
			if(fabs(velocity[1])>V_FILTER_WIDTH)
				shifting[1] += (velocity[1]*t/1000);
			//out<<dataUnit[i].getAcc().getY()<<"\t"<<velocity[1]<<"\t"<<shifting[1]<<"\t";//"\t"<<plus<<
			dataList.getDataUnitThisLoop(i).setV_Y(velocity[1]);
			dataList.getDataUnitThisLoop(i).setS_Y(shifting[1]);

			///////az
			plus = 0.0f;			
			a = (dataUnit[(i-1+5)%5].getAcc().getZ()+dataUnit[i].getAcc().getZ())/2;
			cout<<"cal_az"<<i<<":"<<a<<endl;
			t = dataUnit[i].getTime() - dataUnit[(i-1+5)%5].getTime();
			if(a>=ACC_FILTER_WIDTH||a<=-ACC_FILTER_WIDTH){
				velocity[2] += (a*g*100*t/1000);
				plus = a*g*100*t/1000;
				cout<<"plus_z:"<<plus<<"cm/s"<<endl;
			}
			//if(fabs(velocity[0])<dataList->getStaticWidth())
			if(dataList->isStatic())
				velocity[2] = 0.0f;
			if(fabs(velocity[2])>V_FILTER_WIDTH)
				shifting[2] += (velocity[2]*t/1000);
			//out<<dataUnit[i].getAcc().getZ()<<"\t"<<velocity[2]<<"\t"<<shifting[2]<<"\t";//"\t"<<plus<<
			dataList.getDataUnitThisLoop(i).setV_Z(velocity[2]);
			dataList.getDataUnitThisLoop(i).setS_Z(shifting[2]);

			///////total acc
			// if(isrecord&&out.is_open())
			// 	out<<dataUnit[i].getAcc().getTotalAcc()<<"\t"<<getVelocity()<<"\r\n";


			//////isStatic,vx = vy = vz = 0;

		}

		dataUnit[4].setDataUnit(dataUnit[3]);

		cout<<"-----------------v0:"<<velocity[0]<<endl;




		

		// float a = (lastDataUnit.getAcc().getX()+dataUnit[0].getAcc().getX())/2;
		// float t = dataUnit[0].getTime() - lastDataUnit.getTime();
		// cout<<"time0:"<<dataUnit[0].getTime()<<" lasttime:"<<lastDataUnit.getTime()<<endl;;
		// cout<<"cal_a0:"<<a<<endl;
		// if(a>=FILTER_WIDTH||a<=-FILTER_WIDTH){
		// 	float ttt = a*g*t/1000;
		// 	velocity[0] += (a*g*100*t/1000);
		// 	cout<<"plus:"<<ttt*100<<"cm/s"<<endl;
		// }

		// if(isrecord)
		// 	out<<dataUnit[0].getTime()<<"\t"<<a<<

		// for(int i=1;i<4;++i){
		// 	a = (dataUnit[i-1].getAcc().getX()+dataUnit[i].getAcc().getX())/2;
		// 	cout<<"cal_a"<<i<<":"<<a<<endl;
		// 	t = dataUnit[i].getTime() - dataUnit[i-1].getTime();
		// 	if(a>=FILTER_WIDTH||a<=-FILTER_WIDTH){
		// 		velocity[0] += (a*g*100*t/1000);
		// 		float ttt = a*g*t/1000;
		// 		cout<<"plus:"<<ttt*100<<"cm/s"<<endl;
		// 	}
			
		// }

		//lastDataUnit.setDataUnit(dataUnit[3]);


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

	float getVelocity(){
		return sqrt(velocity[0]*velocity[0]+velocity[1]*velocity[1]+velocity[1]*velocity[1]);
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
	static float velocity[3];
	static float shifting[3];
	DataList *dataList;
	//DataUnit lastDataUnit;
	DataUnit dataUnit[5];//dataunit[4] is lastdataunit
	
};

float DataHandler::velocity[] = {0.0f,0.0f,0.0f};
float DataHandler::shifting[] = {0.0f,0.0f,0.0f};

#endif