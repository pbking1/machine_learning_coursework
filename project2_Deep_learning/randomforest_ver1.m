%datax = load('train.csv');
%datay = load('test.csv');
%datax = load('trainaa');
%datay = load('testaa');
%X = datax(:, 3:3074);
%X = datax(1:100, 3:30);
%Y = datax(1:100, 2);
%Y = datax(:, 2);
%x = datay(:, 2:3073);
Factor=TreeBagger(800,X,Y);
[ds,s] = predict(Factor,x);
ds = cell2mat(ds);
ds = str2num(ds);
fid = fopen('randomf800.csv', 'w');
fprintf(fid, 'Id,label\n');
for i = 1:40000
    fprintf(fid, '%g,%g\n', i, ds(i));
end
fclose(fid);