#include<iostream>
#include<cmath>
#include<fstream>
#include<cstdio>
using namespace std;
int id[362091];

int main(){
	char buffer[256];
	ifstream fin("test.txt");
	double c;
	int id1, a, b, i = 0, j;
	while(!fin.eof()){
		fin.getline(buffer, 50);
		sscanf(buffer, "%d %d %d %lf", &id1, &a, &b, &c);
		id[i] = id1;
		i++;
	}
	fin.close();

	ofstream fout("id_index.csv");
	for(i = 0; i < 362091; i++)
		fout<<id[i]<<endl;
	fout.close();
	return 0;
}
