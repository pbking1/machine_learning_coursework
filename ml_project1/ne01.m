t = csvread('train.csv', 1, 1);
x = t(:, 2:385);  %try multi-dimensional
y = t(:, 1);
nnt = newff(x', y', [22,22,22]);


net.trainParam.epochs = 200;
net.trainParam.lr = 0.01;
net.trainParam.mu = 0.2;
net.trainParam.mu_dec = 0.01;
net.trainParam.mu_inc = 0.01;
net.trainParam.show = 1;
net.trainParam.goal = 1e-7;
nnt = train(nnt, x', y');

Test = csvread('test.csv', 1, 1);
theta = sim(nnt, Test');
sigma = [];
sum1 = sim(nnt, x');
sum2 = sum1';
xx = sum2 - y;
yy = xx .* xx;
sigma = [sigma ; sum(yy)];
sum(yy)

fid = fopen('lr_goalver5.csv', 'w');
fprintf(fid, 'Id,reference\n');
for i = 1:10000
    fprintf(fid, '%g,%g\n', i + 10000, theta(i));
end
fclose(fid);