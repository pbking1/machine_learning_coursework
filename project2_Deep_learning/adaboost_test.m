%clear all;
%clc;
%matlabpool local 2
op.iter = 200;                 % 10 iterartions
ds = Bcl_adaboost(X,Y,x,op); % AdaBoost classifier
fid = fopen('adab200.csv', 'w');
fprintf(fid, 'Id,label\n');
for i = 1:40000
    fprintf(fid, '%g,%g\n', i, ds(i));
end
fclose(fid);
matlabpool close;



