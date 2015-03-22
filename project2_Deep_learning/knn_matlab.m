train_data = load('train.csv');
test_data = load('test.csv');

X = train_data(:, 3:3074);
Y = train_data(:, 2);
x = test_data(:, 2:3073);

label = knnclassify(x,X,Y,10,'correlation','nearest');

fid = fopen('knn_result2.csv', 'w');
fprintf(fid, 'id,label\n');
for i = 1:40000
    fprintf(fid, '%d,%g\n', i, label(i) + 1);
end
fclose(fid);