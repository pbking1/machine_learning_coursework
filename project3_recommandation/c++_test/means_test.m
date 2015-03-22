clear all;clc;
data = load('matrix_data.csv');

item_person = [];
for i = 1:101
    for j = 1:24983
        if(data(i,j) == 0)
            item_person(i,j) = 0;
        else
            item_person(i,j) = data(i,j);
        end
    end
end

id_data = load('id_index.csv');
test_data = load('matrix_test_data.csv');
test_result_data = test_data;

for i = 1:101
    for j = 1:24983
        if(test_data(i,j) == 1)
            test_result_data(i,j) = item_person(i,j);
        end
    end
end
k = 1;
for j = 1:24983
    for i = 1:101
        if(test_result_data(i,j) ~= 0)
            test_result(k) = test_result_data(i,j);
            k = k + 1;
        end
    end
end
rdresult = test_result';

%write to file
fid = fopen('rd_ver11.csv', 'w');
fprintf(fid, 'id,rating\n');
for k = 1:362091
    fprintf(fid, '%d,%g\n', [id_data(k) rdresult(k)]);
end
fclose(fid);



                
                










