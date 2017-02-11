#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void main(){
	ofstream out("E:\\hello.txt");
	if (out.is_open())
		for (int i = 0; i < 100; ++i){
			float a = 0.001*i;
			float b = 0.1*(i % 10);
			out << a << "\t" << b << "\n";
		}
	


	ifstream in("E:\\hello.txt");
	if (in.is_open()){
		string s;
		for (int i = 0; i < 5; ++i){
			getline(in, s);
			if (i > 2)
				cout << s << endl;
		}

		for (int i = 0; i < 3; ++i)
			out << "ssss\n";
	}
	out.close();
	in.close();

	system("pause");
}