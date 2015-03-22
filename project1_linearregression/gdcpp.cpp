#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

#define OUTPUTID 10001
#define BUFFERSIZE 50000
#define ROWNUM 10000
#define COLNUM 385

double alpha = 0.1;
char buffer[BUFFERSIZE];
const char *delim = ",";
double x[ROWNUM][COLNUM];
double y[ROWNUM];
double result[ROWNUM];
double diff[ROWNUM];
double theta[COLNUM];
double temp[COLNUM];

void readdata(char *, bool);
void writedata(char *);
void test();
void gradient_descend_train();

int main(){
	readdata("train.csv", true);
	gradient_descend_train();
	readdata("test.csv", false);
	test();
	writedata("predict.csv");
	return 0;
}

void readdata(char *filename, bool haspredicted){
	FILE *inputfile = fopen(filename, "r");

	if(inputfile == NULL){
		system("PAUSE");
		exit(1);
	}
	//drop the first line
	fscanf(inputfile, "%s", buffer);
	//read all lines each
	char *s;
	for(int i = 0; i < ROWNUM; i++){
		
		fscanf(inputfile, "%s", buffer);
		//drop the first column
		strtok(buffer, delim);
		//read the predict y
		if(haspredicted){
			s = strtok(NULL, delim);
			sscanf(s, "%lf", &y[i]);
		}
		//init x0
		x[i][0] = 1;
		//read the matrix
		for(int j = 1; j < COLNUM; j++){
			s = strtok(NULL, delim);
			sscanf(s, "%lf", &x[i][j]);
		}
	}
	fclose(inputfile);
}

void writedata(char *filename){
	FILE *outputfile = fopen(filename, "w");
	
	if(outputfile == NULL){
		system("pause");
		exit(1);
	}

	fprintf(outputfile, "%s,%s\n", "Id", "reference");
	//write the result into file
	for(int i = 0, id = OUTPUTID; i < ROWNUM; i++, id++){
		//cout<<"write the line"<<i + 1<<endl;
		fprintf(outputfile, "%d,%.6lf\n", id, result[i]);
	}
	fclose(outputfile);
}

void initTheta(){  //init theta
	char *thetafilename = "theta.dat";
	FILE *f = fopen(thetafilename, "r");
	for(int j = 0; j < COLNUM; j++)
		fscanf(f, "%lf", &theta[j]);
	fclose(f);
	//init the theta
	for(int j = 0; j < COLNUM; j++)
		theta[j] = 0;
}

void saveTheta(){   //save the theta
	FILE *f = fopen("theta.dat", "w");
	for(int j = 0; j < COLNUM; j++)
		fprintf(f, "%lf\n", theta[j]);
	fclose(f);
}

void calculateResult(){
	for(int i = 0; i < ROWNUM; i++){
		result[i] = 0;
		for(int j = 0; j < COLNUM; j++){
			result[i] += theta[j] * x[i][j];
		}
	}
}

double calculateJ(){
	int turn = 0;
	double cost = 0;
	for(int i = 0; i < ROWNUM; i++){
		diff[i] = result[i] - y[i];
		cost += diff[i]*diff[i];
	}
	cost /= (ROWNUM * 2);
	printf("%5d: J(theta) = %.6lf\n", ++turn, cost);
	return cost;
}

void updateTheta(){
	double sum;
	for(int j = 0 ; j < COLNUM; j++){
		sum = 0;
		for(int i = 0; i < ROWNUM; i++)
			sum += diff[i] * x[i][j];
		theta[j] -= alpha * sum / ROWNUM;
	}
}

void gradient_descend_train(){
	initTheta();
	alpha = 0.1001;
	double cost = 1000;
	while(cost > 26.4){
		calculateResult();
		cost = calculateJ();
		updateTheta();
	}
	saveTheta();
}

void test(){
	calculateResult();
}

