
data = load('train.txt');
X = data(:, 3:386);
X1 = X .* X;
X = [X X1];
y = data(:, 2);
m = length(y);
m2 = size(X);

data2 = load('test.txt');
feat = data2(:, 2:385);
m3 = size(feat);

sum_test = [0];

%for w = 1:10000

theta = normaleqn(X, y, 10);
%m3 = length(theta);

theta = theta(1:384)
result = feat * theta;
%result
linen = data2(:, 1);
%linen
%m4 = length(result);
%m5 = length(linen);

csvwrite('aaa_ver4.csv', [linen result]);
%csvwrite('origin_test1.csv', X * theta);
%csvwrite('theta-sample.csv', theta);
%ss = 0;
%for i = 1:10000
%    ss = ss + sum((X*theta - y).*(X*theta - y));
%end


%fprintf('\n');
%sum(ss)

%sum_test = [sum_test ; sum(ss)];

%end