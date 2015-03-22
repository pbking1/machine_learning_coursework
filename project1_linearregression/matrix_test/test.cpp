#include<iostream>
#include<ctime>
#include<cmath>
using namespace std;
int a[10][10];
int b[10][10];
int c[10][10];
int inv_c[10][10];

void turn(){
	for(int i = 0; i < 10; i++){
		for(int j =0 ; j < 10; j++){
			b[i][j] = a[j][i];
		}
	}
}

void mul(){
	for(int i = 0; i < 10; i++)
		for(int j = 0; j < 10; j++)
			c[i][j] = 0;
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			c[i][j] += a[i][j]*b[j][i];
		}
	}
}

void inv(){
	double param = 1;
	int matrix_enhance[10][20], temp[20];
	for(int i = 0; i < 10; i++)
		for(int j = 0; j < 10; j++)
			matrix_enhance[i][j] = a[i][j];
	for(int i = 0; i < 10; i++){
		for(int j = 10; j < 20; j++)
			matrix_enhance[i][j] = 0;
		matrix_enhance[i][i + 10] = 1;
	}
	int i, j, k;
	for(i = 0; i < 10; i++){
		if(matrix_enhance[i][i] == 0){
			if(i == 9)
				cout<<"The matrix can not be inv"<<endl;
			for(j  = i; j < 10; j++)
				if(matrix_enhance[j][i] != 0){
					k = j;
					break;
				}
			for(int u = 0; u < 20; u++){
				temp[u] = matrix_enhance[k][u];
				matrix_enhance[k][u] = matrix_enhance[i][u];
				matrix_enhance[i][u] = temp[u];
			}
		}
		for(j = 0; j < 10; j++){
			if(j != i){
				if(matrix_enhance[j][i] != 0){
					param = matrix_enhance[j][i]/matrix_enhance[i][i];
					for(k = i; k < 20; k++)
						matrix_enhance[j][k] -= param * matrix_enhance[i][k];
				}
			}
		}
		param = matrix_enhance[i][i];
		for(j = i; j < 20; j++)
			if(param != 0)
				matrix_enhance[i][j] /= param;
	}
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			inv_c[i][j] = matrix_enhance[i][j + 10];

}

int main(){
	srand(time(NULL));
	for(int i = 0; i < 10; i++)
		for(int j = 0; j < 10; j++)
			a[i][j] = rand() % 10 + 1;
	for(int i = 0; i < 10; i++){
		for(int j =0 ; j < 10; j++){
			cout<<a[i][j]<<" ";
		}
		cout<<endl;
	}

	cout<<endl;
	turn();
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			cout<<b[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	mul();
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			cout<<c[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	inv();
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 10; j++){
			cout<<inv_c[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	return 0;
}
