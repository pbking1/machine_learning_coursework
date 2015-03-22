t = csvread('train.csv', 1, 1);

x_train = t(1:8000, 2:385);
x_test = t(8001:10000, 2:385);
y_train = t(1:8000, 1);
y_test = t(8001:10000, 1);
y = t(:, 1);


%------neutural network-------%
%50 15 30
nnt = newff(x_train', y_train', [95,30,10],{'logsig','logsig','tansig'},'traincgf');
net.layers{1}.initFcn = 'initwb';
net.inputWeights{1,1}.initFcn = 'rands';
net.biases{1,1}.initFcn = 'rands';
net.biases{2,1}.initFcn = 'rands';
net.trainParam.epochs = 1000;
net.trainParam.lr = 2.0;  %1.8
net.trainParam.mu = 0.01; %1.0
%net.trainParam.mu_dec = 0.1;
%net.trainParam.mu_inc = 0.1;
net.trainParam.show = 10;
net.trainParam.max_fail = 10;
%net.trainParam.goal = 1e-7;
net.trainParam.min_grad = 0.005;
net.trainParam.goal = 0;

nnt = train(nnt, x_train', y_train');

result_train = sim(nnt, x_test');

sig = y_test - result_train';
temp = sum(sig .*sig)

Test = csvread('test.csv', 1, 1);
theta = sim(nnt, Test');

%------------write file
fid = fopen('final_result3-8.csv', 'w');
fprintf(fid, 'Id,reference\n');
for i = 1:10000
    fprintf(fid, '%g,%g\n', i + 10000, theta(i));
end
fclose(fid);
%sigma = [];
%sig = [];
%sig = y_test - result_train';
%temp = sum(sig .* sig)

%sigma = [sigma; sum(sig .* sig)  0.1];
