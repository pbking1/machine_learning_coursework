clc;clear all;
aa = csvread('train.txt');
data = aa(:, 2:386);
y_value = data(:, 1);
feature_value = data(:, 2:385);
m = length(y_value);

alpha = 0.01;
iter_number = 3;
[feature1, mu, sigma] = featureNormal(feature_value);
data1 = feature1(:, 1:59);
data2 = feature1(:, 61:69);
data7 = feature1(:, 71:158);
data6 = feature1(:, 160:179);
data3 = feature1(:, 181:189);
data8 = feature1(:, 191:279);
data5 = feature1(:, 281:384);
rq = zeros(10000, 1);
data4 = [data1 rq data2 rq data7 rq data6 rq data3 rq data8 rq data5];
%csvwrite('feature_normalize_test1.csv', data4)


feature = [ones(m, 1) data4];

theta = zeros(385, 1);
[theta, J_history] = gradientdecend(feature, y_value, theta, alpha, iter_number);
theta

