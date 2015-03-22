clc;clear all;
aa = csvread('train.txt');
data = aa(:, 2:386);
y_value = data(:, 1);
feature_value = data(:, 2:385);
csvwrite('train_feature.csv', [feature_value]);
csvwrite('train_y.csv', [y_value]);