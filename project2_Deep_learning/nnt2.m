%>5.20
nnt = newff(X', Y', [1700,700,1700,700,1700,700], {'logsig','tansig','logsig','tansig','logsig','tansig'}, 'traincgb');

net.trainParam.epochs = 10000;
net.trainParam.lr = 0.01;  %1.8
net.trainParam.mu = 0.01; %1.0
net.trainParam.max_fail = 10;
net.trainParam.goal = 0;
net.trainParam.goal = 0;

nnt = train(nnt, X', Y');
result = sim(nnt, x');


fid = fopen('result2.csv', 'w');
fprintf(fid, 'Id,label\n');

for i = 1:40000
    if((round(result(i)) + 1) > 10)
        result(i) = round(rand(0,9));
    else
        result(i) = round(result(i)) + 1;
    end
end


for i = 1:40000
    fprintf(fid, '%g,%g\n', i, result(i));
end
fclose(fid);