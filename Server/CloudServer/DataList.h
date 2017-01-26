#ifndef DATALIST_H
#define DATALIST_H

#include <iostream>
#include "DataUnit.h"
using namespace std;

#define MAX_LENGTH 512

class DataList{
public:
	DataList(){
		length = 0;
		header = 0;
		index = 0;
	}

	static DataList *getInstance(){
		if(list == NULL){
			list = new DataList();
		}
		return list;
	}

	void addDataUnit(int* startPos,int t){
		length++;
		if(length > MAX_LENGTH){
			cout<<"out of length"<<endl;
			return;
		}
		dataList[index++] = DataUnit(startPos,t);
		//cout<<"datalist time:"<<dataList[index-1].getTime()<<endl;
		if(index>=MAX_LENGTH)
			index = 0;
	}

	//delete [start,end)
	void deleteDataUnit(int start,int end){
		header = end;
		int l = ((MAX_LENGTH+end-start)%MAX_LENGTH);
		length -= l;
	}

	int *findOneGesture(){
		for(int i=header;i<header+length;++i){
			//index: i%MAX_LENGTH

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
};

DataList* DataList::list = NULL;

#endif