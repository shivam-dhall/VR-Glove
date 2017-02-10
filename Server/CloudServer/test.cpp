#include <iostream>
#include <fstream>
using namespace std;

int main(){
	ofstream out("test.txt");
	for(int i=0;i<100;++i)
		out<<i<<"\t"<<0.001<<"\r\n";
	out.close();

	return 0;
}