clear all;
clc;
data = csvread('train.csv', 1, 1);
X = data(:, 2:385);
y = data(:, 1);

NeturalNet = newff(X', y', [12,12,12]);
net.trainParam.epochs = 500;
net.trainParam.lr = 0.002;
net.trainParam.goal = 0.2;

NeturalNet = train(NeturalNet, X', y');
data2 = csvread('test.csv', 1, 1);
prediction = sim(NeturalNet, data2');

%result = feat * theta;

%linen = data2(:, 1);
%csvwrite('aaa_ver5.csv', [linen result]);