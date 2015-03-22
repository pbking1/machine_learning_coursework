clear all; close all; clc;
data = load('train.txt');
X = data(:, 3:386);
y = data(:, 2);
m = length(y);

%get the theta
alpha = 0.01;
iter_number = 5;

%[X_n, mu, sigma] = featureNormal(X);

X_n = X;
X_n = [ones(m, 1) X_n];
theta = zeros(size(X, 2), 1);
J_history = ones(iter_number, 1);

%fprintf(' %f \n', theta);

[theta, J_history] = gd(X_n, y, theta, alpha, iter_number);
%unimplemented

%data2 = load('test.txt');
%feat = data2(:, 2:385);
%m3 = size(feat);
%fprintf('The number of test feature is %d\n', m3);
result = X * theta;
%result
%linen = data2(:, 1);
%linen
%m4 = length(result);
%m5 = length(linen);
%fprintf('The number of y is %d and the length of the id is %d\n', m4, m5);

%csvwrite('bbb.csv', [linen result])
