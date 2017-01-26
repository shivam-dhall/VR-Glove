#ifndef DATATYPE_H
#define DATATYPE_H

#include <iostream>
#include <math.h>
using namespace std;
#define PI 3.141592653

//normalise acc, multiply g(9.8m/s2) to get the real value
class Acceleration{
public:
	Acceleration(){
		x = y = z =0.0f;
	}

	Acceleration(int _x,int _y,int _z){

		x = (float)_x/32768*16;
		y = (float)_y/32768*16;
		z = (float)_z/32768*16;
		// x /= norm;
		// y /= norm;
		// z /= norm;
	}

	void setAcc(float _x, float _y,float _z){
		x = _x;
		y = _y;
		z = _z;
	}

	float getX(){
		return x;
	}

	float getY(){
		return y;
	}

	float getZ(){
		return z;
	}

//private:
	float x;
	float y;
	float z;
};

class Angular{
public:
	Angular(){
		x = y = z =0.0f;
	}

	Angular(int _x,int _y,int _z){
		x = (float)_x*2000/32768;
		y = (float)_y*2000/32768;
		z = (float)_z*2000/32768;
	}

	float getX(){
		return x;
	}

	float getY(){
		return y;
	}

	float getZ(){
		return z;
	}

//private:
	float x;
	float y;
	float z; 
};

class Angle{
public:
	Angle(){
		x = y = z =0.0f;
		rad_x = rad_y = rad_z = 0.0f;
	}

	Angle(int _x,int _y,int _z){
		x = (float)_x*180/32768;
		y = (float)_y*180/32768;
		z = (float)_z*180/32768;
		rad_x = (float)_x*PI/32768;
		rad_y = (float)_y*PI/32768;
		rad_z = (float)_z*PI/32768;
		//cout<<"origin:"<<x<<" "<<y<<" "<<z<<", "<<"cal:"<<rad_x<<" "<<rad_y<<" "<<rad_z<<endl;	
		q[0] = cos(rad_x/2)*cos(rad_y/2)*cos(rad_z/2) + sin(rad_x/2)*sin(rad_y/2)*sin(rad_z/2);
		q[1] = sin(rad_x/2)*cos(rad_y/2)*cos(rad_z/2) - cos(rad_x/2)*sin(rad_y/2)*sin(rad_z/2);
		q[2] = cos(rad_x/2)*sin(rad_y/2)*cos(rad_z/2) + sin(rad_x/2)*cos(rad_y/2)*sin(rad_z/2);
		q[3] = cos(rad_x/2)*cos(rad_y/2)*sin(rad_z/2) - sin(rad_x/2)*sin(rad_y/2)*cos(rad_z/2);
		
	}


	float getX(){
		return x;
	}

	float getY(){
		return y;
	}

	float getZ(){
		return z;
	}

	float getRad_X(){
		return rad_x;
	}

	float getRad_Y(){
		return rad_y;
	}

	float getRad_Z(){
		return rad_z;
	}

public:
	float q[4];
private:
	float x;
	float y;
	float z;
	float rad_x;
	float rad_y;
	float rad_z;
	
};

#endif