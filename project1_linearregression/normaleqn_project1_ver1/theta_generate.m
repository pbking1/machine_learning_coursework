
data = load('train.txt');
X = data(:, 3:386);
y = data(:, 2);
m = length(y);
m2 = size(X);

data2 = load('test.txt');
feat = data2(:, 2:385);
m3 = size(feat);

sum_test = [0];

for w = 1:10000

theta = normaleqn(X, y, w);
%m3 = length(theta);

%result = feat * theta;
%result
%linen = data2(:, 1);
%linen
%m4 = length(result);
%m5 = length(linen);

%csvwrite('aaa_ver3.csv', [linen result]);
%csvwrite('origin_test.csv', X * theta);

ss = 0;
%for i = 1:10000
    ss = ss + sum((X*theta - y).*(X*theta - y));
%end

w
fprintf('\n');
sum(ss)

sum_test = [sum_test ; sum(ss)];

end