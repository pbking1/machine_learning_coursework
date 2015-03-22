t = csvread('train.csv', 1, 1);

x_train = t(1:8000, 2:385);
x_test = t(8001:10000, 2:385);
y_train = t(1:8000, 1);
y_test = t(8001:10000, 1);
mu_best = 0;
sigma_mi = 1;

for i = 0:100
nnt = newff(x_train', y_train', [22,22,22]);
net.trainParam.epochs = 100;
net.trainParam.lr = 0.1;
net.trainParam.mu = 1.1;
net.trainParam.mu_dec = 0.01 + i * 0.01;
net.trainParam.mu_inc = 0.01;
net.trainParam.show = 1;
net.trainParam.goal = 1e-5;
nnt = train(nnt, x_train', y_train');
result_train = sim(nnt, x_test');

sigma = [100 100];
sig = [];
sig = y_test - result_train';
temp = sum(sig .*sig);
if(sigma_mi > temp)
    sigma_mi = temp;
    mu_best = 0.01 + i * 0.01;
end
sigma = [sigma ; temp 0.01 + i * 0.01]

end
%---------final data-----%

nnt = newff(x_train', y_train', [22,22,22]);
net.trainParam.epochs = 200;
net.trainParam.lr = 0.1;
net.trainParam.mu = 1.1;
net.trainParam.mu_dec = mu_best;
net.trainParam.mu_inc = 0.01;
net.trainParam.show = 1;
net.trainParam.goal = 1e-6;
nnt = train(nnt, x_train', y_train');

Test = csvread('test.csv', 1, 1);

theta = sim(nnt, Test');

%------------write file
fid = fopen('final_result3.csv', 'w');
fprintf(fid, 'Id,reference\n');
for i = 1:10000
    fprintf(fid, '%g,%g\n', i + 10000, theta(i));
end
fclose(fid);