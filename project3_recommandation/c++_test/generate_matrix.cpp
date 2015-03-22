#include<iostream>
#include<cmath>
#include<fstream>
#include<cstdio>
using namespace std;
double train_data[103][24984];

int main(){

	for(int i = 0; i < 103; i++)
		for(int j = 0; j < 24984; j++)
			train_data[i][j] = 0;

	char buffer[256];
	ifstream fin("train.txt");
	double c;
	int id, a, b;
	int i, j;
	while(!fin.eof()){
		fin.getline(buffer, 50);
		sscanf(buffer, "%d %d %d %lf", &id, &a, &b, &c);
		train_data[b][a] = c;
	}
	fin.close();
/*
	double mean[24984], sum[24984];
	int count[24984];
	for(i = 0; i < 24984; i++){
		mean[i] = 0;
		sum[i] = 0;
		count[i] = 0;
	}

	for(i = 1; i < 24984; i++){
		for(j = 1; j < 102; j++){
			if(train_data[j][i] != 0){
				count[i]++;
				sum[i] += train_data[j][i];
			}
		}
	}
	for(i = 1; i < 24984; i++){
		mean[i] = sum[i] / count[i];
	}

	for(i = 1; i < 102; i++){
		for(j = 1; j < 24984; j++){
			if(train_data[i][j] == 0)
				train_data[i][j] = mean[j] * (count[j]/101.0);
		}
	}
*/
	ofstream fout("matrix_data.csv");
	if(!fout)
		cout<<"error";
	for(int i = 1; i < 102; i++){
		for(int j = 1; j < 24984; j++){
			if(j != 24983)
				fout << train_data[i][j] <<",";
			else
				fout << train_data[i][j];
		}
		fout<<endl;
	}
	fout.close();

	return 0;
}
