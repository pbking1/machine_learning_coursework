clc;clear all;
aa = csvread('train.txt');
data = aa(:, 2:386);
y = data(:, 1);
X = data(:, 2:385);
[b,bint,r,rint,stats] = regress(y,X);

data2 = load('test.txt');
feat = data2(:, 2:385);
result = feat * b;
linen = data2(:, 1);
csvwrite('ttt.csv', [linen result]);
