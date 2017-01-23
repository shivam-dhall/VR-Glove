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
		// cout<<"_init:"<<cnt<<endl;
		// if(cnt<200){
		// 	float total = sqrt(acc.x*acc.x+acc.y*acc.y+acc.z*acc.z);
		// 	g += total;
		// 	cout<<"total:"<<total<<" temp_g:"<<g<<endl;
		// 	++cnt;
		// }
		// else if(cnt == 200){
		// 	++cnt;
		// 	g/=200;
		// }
		// else
			_eliminateAcc();
	}

	void _eliminateAcc(){	
		//cout<<"g:"<<g<<endl;
		cout<<"x:"<<acc.x<<" y:"<<acc.y<<" z:"<<acc.z<<endl;
		//cout<<"cos cal-x:"<<(total*cos(angle.rad_x))<<" cal-y:"<<(total*cos(angle.rad_y))<<" cal-z:"<<(total*cos(angle.rad_z))<<endl;
		//cout<<"sin cal-x:"<<(-g*sin(angle.rad_y))<<" cal-y:"<<(-g*sin(angle.rad_x))<<endl;//<<" cal-z:"<<(total*sin(angle.rad_z))<<endl;	
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

