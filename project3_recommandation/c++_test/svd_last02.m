data = load('matrix_data.csv');
%matlabpool local 2;
%replace the unjudged data
item_person = [];
parfor i = 1:101
    %mean = sum(data(:,i))/101.0;
    for j = 1:24983
        if(data(i,j) == 0)
            item_person(i,j) = -0.2;
        else
            item_person(i,j) = data(i,j);
        end
    end
end

%calculate the similarity
%????????????matlab????????corrcoef(x)??x??????????????????????
cos_val = corrcoef(item_person');

%generate mean of the mark
sum = [];
average_item = [];
parfor i = 1:101
    s = 0;
    n = 0;
    for j = 1:24983
        if(item_person(i,j) ~= -0.2)
            s = s + item_person(i,j);
            n = n +1;
        end
    end
    average_item(i) = s/n;
end

parfor i = 1:101
    for j = 1:24983
        item_person1(i,j) = item_person(i,j) - average_item(i);
    end
end

%generate the mark( by the items whose |similarity|>0.1)
cos_val1 = [];
parfor i = 1:101
    for j =1:101
        if(abs(cos_val(i,j)) > 0.1)
            cos_val1(i,j) = cos_val(i,j);
        else
            cos_val1(i,j) = 0;
        end
    end
end
                
result = [];
result = cos_val1 * item_person1;

final_result = [];
parfor i = 1:101
    for j = 1:24983
        final_result(i,j) = result(i,j) / sum(abs(cos_val1(i,:)));
        final_result(i,j) = final_result(i,j) + average_item(i);
    end
end

id_data = load('id_index.csv');

test_data = load('matrix_test_data.csv');
test_result_data = test_data;
%check the answer if exist already
parfor i = 1:101
    %mean = sum(data(:,i))/101.0;
    for j = 1:24983
        if(test_data(i,j) == 1 && item_person(i,j) ~= -0.2)
            test_result_data(i,j) = data(i,j);
        end
    end
end

%get the predicted answer
parfor i = 1:101
    for j = 1:24983
        if(test_result_data(i,j) == 1)
            test_result_data(i,j) = final_result(i,j);
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
fid = fopen('mml1.csv', 'w');
fprintf(fid, 'id,rating\n');
for i = 1:362091
    fprintf(fid, '%d,%g\n', [id_data(i) rdresult(i)]);
end
fclose(fid);

matlabpool close;





