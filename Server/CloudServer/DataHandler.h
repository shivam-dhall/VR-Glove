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
#define ZAngle_FILTER_WIDTH 1.8
#define CNT 40

class DataHandler{
public:
	DataHandler(){
		dataUnit[4] = DataUnit();
		dataList = DataList::getInstance();
		zAngle = 0.0f;
		modifyShifting[0] = modifyShifting[1] = modifyShifting[2] = 0.0f;
		for(int i = 0;i<10;++i){
			max_resistance[i] = -1000000.0f;
			min_resistance[i] = 1000000.0f;
		}
		for(int i=0;i<5;++i)
			fingerState[i] = 0;
	}

	void setRecvData(int d, int i){
		recvData[i] = d;
	}

	void closeFile(ofstream &out){
		if (out.is_open())
			out.close();
	}

	void handleRecvData(ofstream &out,bool isrecord,int cnt){
		//
		// float refer_resistance[10] = {
		// 	30000.0f,70000.0f,160000.0f,140000.0f,70000.0f,
		// 	50000.0f,170000.0f,80000.0f,170000.0f,100000.0f
		// };
		int time[4];
		time[0] = recvData[46];
		time[1] = recvData[46]+recvData[47];
		time[2] = recvData[46]+recvData[48];
		time[3] = recvData[46]+recvData[49];

		cout<<"recvdata:";
		for(int i=0;i<10;++i)
			cout<<recvData[i]<<" ";
		cout<<endl;
		float const_res = 20000.0f;
		float curr1 = recvData[3]/const_res;

		float r[10];
		r[0] = (recvData[2] - recvData[3])/curr1;
		r[1] = (recvData[1] - recvData[2])/curr1;
		r[2] = (recvData[0] - recvData[3])/curr1;
		r[3] = (27000 - recvData[1])/curr1;

		float curr2 = recvData[9]/const_res;
		r[4] = (recvData[8] - recvData[9])/curr2;
		r[5] = (recvData[7] - recvData[8])/curr2;
		r[6] = (recvData[6] - recvData[7])/curr2;
		r[7] = (recvData[5] - recvData[6])/curr2;
		r[8] = (recvData[4] - recvData[5])/curr2;
		r[9] = (1024 - recvData[4])/curr2;

		if(cnt<CNT)
			for(int i=0;i<10;++i){
				if(r[i]>max_resistance[i])
					max_resistance[i] = r[i];
				if(r[i]<min_resistance[i])
					min_resistance[i] = r[i];
				refer_resistance[i] += r[i];
			}
		else if(cnt==CNT)
			for(int i=0;i<10;++i)
				refer_resistance[i] /= CNT;

		for(int i=0;i<10;++i)
			if(cnt>=CNT){
				if(r[i]<max_resistance[i]&&r[i]>1000)
					resistance[i] = r[i];
			}
			else
				resistance[i] = r[i];
			
		cout<<"resistance:";
		for(int i=0;i<10;++i)
			cout<<resistance[i]<<" ";
		cout<<endl;

		out<<time[0]<<"\t";
		for(int i=0;i<5;++i)
			if(resistance[i]<min_resistance[i]*0.95){
				out<<0<<"\t";
				fingerState[i] = 1;
			}
			else{
				out<<resistance[i]<<"\t";
				fingerState[i] = 0;
			}
		// if(resistance[9]<min_resistance[9])
		// 	out<<0<<"\r\n";
		// else
		// 	out<<resistance[9]<<"\r\n";
		//if(cnt>=CNT)
		for(int i=0;i<5;++i)
				out<<(resistance[i]/min_resistance[i])<<"\t";
		out<<"\r\n";
		



		for(int i=0;i<4;++i){
			dataUnit[i] = DataUnit(recvData+(i*9+10),time[i]);
		}

		///////////////////////////////handle acc
		float a;
		float t;
		for(int i=0;i<4;++i){
			dataList->addDataUnit(recvData+(i*9+10),time[i]);
			dataList->setTotalVelocity(getVelocity());
			///////ax
			float tempa = dataUnit[i].getAcc().getX() - dataUnit[(i-1+5)%5].getAcc().getX();
			if(tempa>0.105 && tempa<0.14){
				dataUnit[i].getAcc().setAcc(dataUnit[i].getAcc().getX()-tempa,dataUnit[i].getAcc().getY(),dataUnit[i].getAcc().getZ());
			}
			else if(tempa>=0.14){
				if(i<3){
					float _x = (dataUnit[(i-1+5)%5].getAcc().getX()+dataUnit[i+1].getAcc().getX())/2;
					dataUnit[i].getAcc().setAcc(_x,dataUnit[i].getAcc().getY(),dataUnit[i].getAcc().getZ());
				}
				else{
					dataUnit[i].getAcc().setAcc(dataUnit[i].getAcc().getX()-tempa,
						dataUnit[i].getAcc().getY(),dataUnit[i].getAcc().getZ());
				}
			}

			a = (dataUnit[(i-1+5)%5].getAcc().getX()+dataUnit[i].getAcc().getX())/2;
			//--cout<<"cal_ax"<<i<<":"<<a<<endl;
			t = dataUnit[i].getTime() - dataUnit[(i-1+5)%5].getTime();
			float plus = 0.0f;
			if(a>=ACC_FILTER_WIDTH||a<=-ACC_FILTER_WIDTH){
				velocity[0] += (a*g*100*t/1000);
				
				plus = a*g*100*t/1000;
				//--cout<<"plus_x:"<<plus<<"cm/s"<<endl;
			}
			//if(fabs(velocity[0])<dataList->getStaticWidth())
			if(dataList->isStatic())
				velocity[0] = 0.0f;
			if(fabs(velocity[0])>V_FILTER_WIDTH){
				shifting[0] += (velocity[0]*t/1000);
				modifyShifting[0] += (velocity[0]*t/1000);
			}
			// if(isrecord&&out.is_open())
			// 	out<<dataUnit[i].getTime()<<"\t"<<dataUnit[i].getAcc().getX()<<"\t"<<velocity[0]<<"\t"<<shifting[0]<<"\t";//"\t"<<plus<<
			// dataList->getDataUnitThisLoop(i).setV_X(velocity[0]);
			// dataList->getDataUnitThisLoop(i).setS_X(shifting[0]);


			///////ay
			plus = 0.0f;
			a = (dataUnit[(i-1+5)%5].getAcc().getY()+dataUnit[i].getAcc().getY())/2;
			//--cout<<"cal_ay"<<i<<":"<<a<<endl;
			t = dataUnit[i].getTime() - dataUnit[(i-1+5)%5].getTime();
			if(a>=ACC_FILTER_WIDTH||a<=-ACC_FILTER_WIDTH){
				velocity[1] += (a*g*100*t/1000);
				
				plus = a*g*100*t/1000;
				//--cout<<"plus_y:"<<plus<<"cm/s"<<endl;
			}
			//if(fabs(velocity[0])<dataList->getStaticWidth())
			if(dataList->isStatic())
				velocity[1] = 0.0f;
			if(fabs(velocity[1])>V_FILTER_WIDTH){
				shifting[1] += (velocity[1]*t/1000);
				modifyShifting[1] += (velocity[1]*t/1000);
			}
			//out<<dataUnit[i].getAcc().getY()<<"\t"<<velocity[1]<<"\t"<<shifting[1]<<"\t";//"\t"<<plus<<
			// dataList->getDataUnitThisLoop(i).setV_Y(velocity[1]);
			// dataList->getDataUnitThisLoop(i).setS_Y(shifting[1]);

			///////az
			plus = 0.0f;			
			a = (dataUnit[(i-1+5)%5].getAcc().getZ()+dataUnit[i].getAcc().getZ())/2;
			//--cout<<"cal_az"<<i<<":"<<a<<endl;
			t = dataUnit[i].getTime() - dataUnit[(i-1+5)%5].getTime();
			if(a>=ACC_FILTER_WIDTH||a<=-ACC_FILTER_WIDTH){
				velocity[2] += (a*g*100*t/1000);
				
				plus = a*g*100*t/1000;
				//--cout<<"plus_z:"<<plus<<"cm/s"<<endl;
			}
			//if(fabs(velocity[0])<dataList->getStaticWidth())
			if(dataList->isStatic())
				velocity[2] = 0.0f;
			if(fabs(velocity[2])>V_FILTER_WIDTH){
				shifting[2] += (velocity[2]*t/1000);
				modifyShifting[2] += (velocity[2]*t/1000);
			}
			//out<<dataUnit[i].getAcc().getZ()<<"\t"<<velocity[2]<<"\t"<<shifting[2]<<"\t";//"\t"<<plus<<
			// dataList->getDataUnitThisLoop(i).setV_Z(velocity[2]);
			// dataList->getDataUnitThisLoop(i).setS_Z(shifting[2]);

			///////total acc
			// if(isrecord&&out.is_open())
			// 	out<<dataUnit[i].getAcc().getTotalAcc()<<"\t"<<getVelocity()<<"\t"<<dataUnit[i].getAngular().getZ()<<"\r\n";

		}
		//--cout<<"-----------------v0:"<<velocity[0]<<endl;

		int zCnt = 0;
		for(int i=0;i<4;++i){
			if(fabs(dataUnit[i].getAngular().getZ())>ZAngle_FILTER_WIDTH)
				++zCnt;
		}

		if(zCnt>3)
			zAngle = dataUnit[3].getAngle().getZ();

		dataUnit[4].setDataUnit(dataUnit[3]);

	}

	int *getFingerState(){
		return fingerState;
	}


	float* getModifyShifting(){
		return modifyShifting;
	}


	float getXAngle(){
		return dataUnit[3].getAngle().getX();
	}

	float getYAngle(){
		return dataUnit[3].getAngle().getY();
	}

	float getZAngle(int cnt){
		if(cnt>2)
			return zAngle;
		else
			return dataUnit[3].getAngle().getZ();
	}

	void return2Zero(){
		modifyShifting[0] = modifyShifting[1] = modifyShifting[2] = 0.0f;
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
	float zAngle;
	float modifyShifting[3];
	float resistance[10];
	float refer_resistance[10];
	float max_resistance[10];
	float min_resistance[10];
	int fingerState[5];
};

float DataHandler::velocity[] = {0.0f,0.0f,0.0f};
float DataHandler::shifting[] = {0.0f,0.0f,0.0f};

#endif