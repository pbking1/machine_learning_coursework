id_data = load('id_index.csv');

test_data = load('matrix_test_data.csv');
test_result_data = test_data;
%check the answer if exist already
for i = 1:101
    %mean = sum(data(:,i))/101.0;
    for j = 1:24983
        if(test_data(i,j) == 1 && item_person(i,j) ~= -0.2)
            test_result_data(i,j) = data(i,j);
        end
    end
end

%get the predicted answer
for i = 1:101
    for j = 1:24983
        if(test_result_data(i,j) == 1)
            test_result_data(i,j) = real_final(i,j);
        end
    end
end

test_result = [];
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
fid = fopen('svdver1.csv', 'w');
fprintf(fid, 'id,rating\n');
for i = 1:362091
    fprintf(fid, '%d,%g\n', [id_data(i) rdresult(i)]);
end
fclose(fid);



                
                