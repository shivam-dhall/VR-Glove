#include <iostream>
#include <math.h>
using namespace std;
#define PI 3.141592653

class Acceleration{
public:
	Acceleration(){}

	Acceleration(int _x,int _y,int _z){
		x = (float)_x*16/32768;
		y = (float)_y*16/32768;
		z = (float)_z*16/32768;
	}

//private:
	float x;
	float y;
	float z;
};

class Angular{
public:
	Angular(){}

	Angular(int _x,int _y,int _z){
		x = (float)_x*2000/32768;
		y = (float)_y*2000/32768;
		z = (float)_z*2000/32768;
	}

//private:
	float x;
	float y;
	float z; 
};

class Angle{
public:
	Angle(){}

	Angle(int _x,int _y,int _z){
		x = (float)_x*180/32768;
		y = (float)_y*180/32768;
		z = (float)_z*180/32768;
		rad_x = (float)_x*PI/32768;
		rad_y = (float)_y*PI/32768;
		rad_z = (float)_z*PI/32768;
		cout<<"origin:"<<x<<" "<<y<<" "<<z<<", "<<"cal:"<<rad_x<<" "<<rad_y<<" "<<rad_z<<endl;
	}

//private:
	float x;
	float y;
	float z;
	float rad_x;
	float rad_y;
	float rad_z;
};