#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<cmath>
#include<stdlib.h>
using namespace std;
int _count  = 0;
double a = 0.01;
double dd = 0.000001;
double data[10000][400];//输入层
double hidelayer[400];//隐藏层
const int layersize = 3;
double u = 0.000001;
double w[layersize-1][400][400];  
void initial(){
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 400; j++){
            for(int k = 0; k < 400; k++){
                double num = rand() % 1000;
                w[i][j][k] = num / 10000;
            }
        }
    }
}
double f(double x){
    return 1 / (1 + exp(-x));
}
double forward(double* in,double* layer2,int size){//前向传递获取每组输入的预测值
    double sum = 0;
    for(int j = 0; j < size-3; j++){
        sum = 0;
        for(int i = 2; i < size; i++){
            sum += in[i] * w[0][i-2][j];
        }
        layer2[j] = f(sum);
    }
    layer2[size-3] = 1;
    double y = 0;
    for(int i = 0; i < size-2; i++){
        y += layer2[i] * w[1][i][0];
    }
    return y;
}
double forward1(double* in,double* layer2,int size){//前向传递获取每组输入的预测值
    double sum = 0;
    for(int j = 0; j < size-2; j++){
        sum = 0;
        for(int i = 1; i < size; i++){
            sum += in[i] * w[0][i-1][j];
        }
        layer2[j] = f(sum);
    }
    layer2[size-2] = 1;
    double y = 0;
    for(int i = 0; i < size-1; i++){
        y += layer2[i] * w[1][i][0];
    }
    return y;
}
double error[400];
double hideerror[400][400];
void bp(int size){
    
    memset(error,0,sizeof(error));
    memset(hideerror,0,sizeof(hideerror));
    for(int i = 0; i < _count; i++){
        double y = forward(data[i],hidelayer,size);
        for(int j = 0; j < size - 2; j++){
            error[j] = (y - data[i][1])*hidelayer[j];
        }
        for(int j = 0; j < size - 2; j++){
            for(int k = 0; k < size - 3; k++){
                hideerror[j][k] = (y - data[i][1])*w[1][k][0]*(1-hidelayer[k])*hidelayer[k]*data[i][j+2];
            }
        }
        for(int j = 0; j < size - 3; j++){
            for(int k = 0; k < size - 3; k++){
                w[0][j][k] -= a * (hideerror[j][k] + (u * w[0][j][k]/_count));
            }
        }
        for(int j = 0; j < size - 3; j++)
            w[0][size - 3][j] -= a * (hideerror[size-3][j]);
        for(int j = 0; j < size - 3; j++){
            w[1][j][0] -= a * (error[j] + (u * w[1][j][0]/_count));
        }
        w[1][size-3][0] -= a * (error[size-3]);
    }
}
double costvalue(int size){
    double cost = 0;
    for(int i = 0; i < _count; i++){
        double y = forward(data[i],hidelayer,size);
        cost += (data[i][1] - y) * (data[i][1] - y);
    }
    cost /= _count;
    return sqrt(cost);
}
int main(){
    ifstream in("train.csv");
    ifstream test("test.csv");
    ofstream out("result.csv");
    char value[5000];
    const int max = 5000;
    int j;
    in.getline(value,max);
    while(in.getline(value,max)){
        string str = "";
        j = 0;
        for(int i = 0; value[i] != '\0'; ++i){
            if(value[i] == ','){
                data[_count][j] = atof(&str[0]);
                str = "";
                j++;
            }
            else{
                str += value[i];
            }
        }
        data[_count][j] = atof(&str[0]);
        j++;
        data[_count][j] = 1;
        j++;
        _count++;
    }
    in.close();
    for(int i = 0; i < _count; ++i)
        cout<<data[i][0]<<" "<<data[i][j-1]<<endl;
    cout<<j<<endl;
    cout<<"数据载入完成"<<endl;
    initial();
    double x1 = 0;
    double x2 = 0;
    for(int i = 0; i < 36; i++){
        x1 = costvalue(j);
        if(x1 > x2 && i > 0)
            a /= 2;
        cout<<i<<" : "<<x1<<endl;
        x2 = x1;
        bp(j);
    }
    cout<<"训练完毕"<<endl;
    _count = 0;
    test.getline(value,max);
        while(test.getline(value,max)){
            string str = "";
            j = 0;
            for(int i = 0; value[i] != '\0'; ++i){
                if(value[i] == ','){
                    data[_count][j] = atof(&str[0]);
                    str = "";
                    j++;
                }
                else{
                    str += value[i];
                }
            }
            data[_count][j] = atof(&str[0]);
            j++;
            data[_count][j] = 1;
            j++;
            _count++;
        }
    cout<<j<<endl;
    test.close();
    out<<"Id,reference"<<endl;
    for(int i = 0; i < _count; i++){
        out<<i+10001<<","<<forward1(data[i],hidelayer,j)<<endl;
    }
    out.close();
    return 0;
}
