#include <iostream>
#include <math.h>
#include "DataType.h"
using namespace std;
#define PI 3.141592653


class DataUnit{
public:
	DataUnit(){}

	//startPoint is the 9 int data's start position,
	//we assume we use 9 data from the start position by default
	DataUnit(int* startPos){
		acc = Acceleration(startPos[0],startPos[1],startPos[2]);
		angular = Angular(startPos[3],startPos[4],startPos[5]);
		angle = Angle(startPos[6],startPos[7],startPos[8]);
		//g = 0;
		_init();
	}

	~DataUnit(){

	}
private:
	void _init(){
			_eliminateAcc();
	}

	void _eliminateAcc(){	
		//cout<<"g:"<<g<<endl;
		cout<<"x:"<<acc.x<<" y:"<<acc.y<<" z:"<<acc.z<<endl;
		float temp_g[3];
		temp_g[0] = 2 * (angle.q[1] * angle.q[3] - angle.q[0] * angle.q[2]);
		temp_g[1] = 2 * (angle.q[0] * angle.q[1] + angle.q[2] * angle.q[3]);
		temp_g[2] = angle.q[0] * angle.q[0] - angle.q[1] * angle.q[1] - angle.q[2] * angle.q[2] + angle.q[3] * angle.q[3];
		cout<<"excellent solution:"<<temp_g[0]<<" "<<temp_g[1]<<" "<<temp_g[2]<<endl;
	}

	Acceleration acc;
	Angular angular;
	Angle angle;
	static float g;
	static int cnt;
};

int DataUnit::cnt = 0;
float DataUnit::g = 0;

