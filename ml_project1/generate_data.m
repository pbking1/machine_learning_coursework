clear all; clc;
t = csvread('train.csv', 1, 1);
x = t(:, 2:385);  %try multi-dimensional
x1 = x .* x;
x2 = x1 .* x1; 
x = [x x1 x2];
y = t(:, 1);