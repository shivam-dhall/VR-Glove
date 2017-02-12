#ifndef DATALIST_H
#define DATALIST_H

#include <iostream>
#include <math.h>
#include "DataUnit.h"
using namespace std;

#define MAX_LENGTH 512
#define TOTAL_STATIC_WIDTH 0.02
#define X_STATIC_WIDTH 0.004
#define Y_STATIC_WIDTH 0.003
#define V_STATIC_WIDTH 3.5
#define V_STATIC_WIDTH_PER_CNT 2
#define STATIC_CNT 10
#define NOT_STATIC_CNT 2

class DataList{
public:
	DataList(){
		length = 0;
		header = 0;
		index = 0;
		staticCnt = 0;
		notStaticCnt = 0;
		totalVelocity = 0.0f;
	}

	static DataList *getInstance(){
		if(list == NULL){
			list = new DataList();
		}
		return list;
	}

	DataUnit& getDataUnitThisLoop(int i){
		return dataList[(index+MAX_LENGTH-(4-i))%MAX_LENGTH];
	}

	void addDataUnit(int* startPos,int t){
		dataList[index++] = DataUnit(startPos,t);
		length++;

		if(fabs(dataList[index-1].getAcc().getTotalAcc())<=TOTAL_STATIC_WIDTH
			//&&fabs(dataList[index-1].getAcc().getX())<=X_STATIC_WIDTH
			//&&fabs(dataList[index-1].getAcc().getX())<=Y_STATIC_WIDTH
			//&&fabs(totalVelocity)<=V_STATIC_WIDTH
			)
		{
			++staticCnt;
			if(fabs(dataList[index-1].getAcc().getTotalAcc())<=(TOTAL_STATIC_WIDTH/2))
				++staticCnt;
			if(staticCnt >= STATIC_CNT)
				notStaticCnt = 0;
		}
		else{
			++notStaticCnt;
			if(staticCnt>=STATIC_CNT){
				deleteDataUnit(header,index);
				staticCnt = 0;
				notStaticCnt = 0;
			}
			if(notStaticCnt>=NOT_STATIC_CNT)
				staticCnt = 0; 

		}

		if(index>=MAX_LENGTH)
		 	index = 0;


		if(length > MAX_LENGTH){
			cout<<"out of length"<<endl;
			return;
		}

	}

	void setTotalVelocity(float v){
		totalVelocity = v;
	}

	
	//delete [start,end)
	void deleteDataUnit(int start,int end){
		header = end;
		int l = ((MAX_LENGTH+end-start)%MAX_LENGTH);
		length -= l;
	}

	float getStaticWidth(){
		if(staticCnt<7)
			return staticCnt*V_STATIC_WIDTH_PER_CNT;
		else
			return 100;
	}

	bool isStatic(){
		if(staticCnt>=STATIC_CNT){
			//staticCnt = 0;
			return true;
		}
		return false;
	}

	int *findOneGesture(){
		for(int i=header;i<header+length;++i){
			//index: i%MAX_LENGTH

			//
			//
			//
			//

		}
		return find_gesture;
	}


private:
	int length;
	int header;
	int index;
	int find_gesture[2];//start & length
	DataUnit dataList[MAX_LENGTH];
	static DataList *list;
	int staticCnt;
	int notStaticCnt;
	float totalVelocity;
};

DataList* DataList::list = NULL;

#endif