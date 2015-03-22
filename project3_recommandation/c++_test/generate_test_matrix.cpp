#include<iostream>
#include<cmath>
#include<fstream>
#include<cstdio>
using namespace std;
double test_data[103][24984];

void init(){
	for(int i = 0; i < 103; i++)
		for(int j = 0; j < 24984; j++)
			test_data[i][j] = 0;
}

int main(){
	init();
	char buffer[256];
	ifstream fin("test.txt");
	double c;
	int id, a, b;
	int i, j;
	while(!fin.eof()){
		fin.getline(buffer, 50);
		sscanf(buffer, "%d %d %d", &id, &a, &b);
		test_data[b][a] = 1;
	}
	fin.close();

	ofstream fout("matrix_test_data.csv");
	for(int i = 1; i < 102; i++){
		for(int j = 1; j < 24984; j++){
			if(j != 24983)
				fout<< test_data[i][j]<<",";
			else
				fout<<test_data[i][j];
		}
		fout<<endl;
	}
	fout.close();
	return 0;
}
