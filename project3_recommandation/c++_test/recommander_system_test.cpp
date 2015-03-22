#include<iostream>
#include<fstream>
#include<cstdio>
#include<cmath>
#include<iomanip>
using namespace std;
double train_data[101][24983];
int train_x[101][101];
double result[101][24983];

int main(){
	char buffer[256];
	ifstream fin("train.txt");
	double c;
	int id, a, b;
	int i, j;
	while(!fin.eof()){
		fin.getline(buffer, 50);
		sscanf(buffer, "%d %d %d %lf",&id, &a, &b, &c);
		train_data[b][a] = c;
	}
	fin.close();

	for(i = 0; i < 101; i++){
		for(j = 0; j < 101; j++){
			train_x[i][j] = 0;
		}
	}
	
	int count = 0;
	int person;
	for(int n = 0; n < 101; n++){
		for(int m = n + 1; m < 101; m++){
			for(person = 0; person < 24983; person++){
				if(train_data[m][person] != 0 && train_data[n][person] != 0){
					count++;
				}
			}
			train_x[m][n] = count;
			train_x[n][m] = count;
			count = 0;
		}
	}
		
	/*for(i = 0; i < 101; i++){
		for(j = 0; j < 101; j++){
			cout<<train_x[i][j]<<" ";
		}
		cout<<endl;
	}*/

	int k = 0;
	for(i = 0; i < 101; i++){
		for(j = 0; j < 24983; j++){
			for(k = 0; k < 101; k++){
				result[i][j] += train_x[i][k] * train_data[k][j];
			}
		}
	}

	/*
	for(i = 0; i < 101; i++){
		for(j = 0; j < 24983; j++){
			cout<<result[i][j]<<" ";
		}
		cout<<endl;
	}*/

	struct final_answer{
		int id;
		double rating;
	};
	final_answer fans[362092];
	int _id, _user_id, _item_id;
	ifstream fin1("test.txt");
	while(!fin1.eof()){
		sscanf(buffer, "%d %d %d", &_id, &_user_id, &_item_id);
		if(train_data[_user_id][_item_id] != 0){
			fans[i].id = _id;
			fans[i].rating = train_data[_user_id][_item_id];
		}else{
			fans[i].id = _id;
			fans[i].rating = result[_user_id][_item_id] / 1000000;
		}
	}

	for(i = 0; i < 362092; i++){
		cout<<fans[i].id<<" "<<fans[i].rating<<endl;
	}
	return 0;
}
