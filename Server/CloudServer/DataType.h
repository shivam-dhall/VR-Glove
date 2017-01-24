#include <iostream>
#include <math.h>
using namespace std;
#define PI 3.141592653

//normalise acc, multiply g(9.8m/s2) to get the real value
class Acceleration{
public:
	Acceleration(){}

	Acceleration(int _x,int _y,int _z){

		x = (float)_x/32768*16;
		y = (float)_y/32768*16;
		z = (float)_z/32768*16;
		float norm = sqrt(x*x+y*y+z*z);
		x /= norm;
		y /= norm;
		z /= norm;
	}

	void setAcc(float _x, float _y,float _z){
		x = _x;
		y = _y;
		z = _z;
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
		q[0] = cos(rad_x/2)*cos(rad_y/2)*cos(rad_z/2) + sin(rad_x/2)*sin(rad_y/2)*sin(rad_z/2);
		q[1] = sin(rad_x/2)*cos(rad_y/2)*cos(rad_z/2) - cos(rad_x/2)*sin(rad_y/2)*sin(rad_z/2);
		q[2] = cos(rad_x/2)*sin(rad_y/2)*cos(rad_z/2) + sin(rad_x/2)*cos(rad_y/2)*sin(rad_z/2);
		q[3] = cos(rad_x/2)*cos(rad_y/2)*sin(rad_z/2) - sin(rad_x/2)*sin(rad_y/2)*cos(rad_z/2);
		
	}

//private:
	float x;
	float y;
	float z;
	float rad_x;
	float rad_y;
	float rad_z;
	float q[4];
};