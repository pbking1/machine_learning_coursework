clc;clear all;
data = load('matrix_data.csv');

%replace the unjudged data
mean = [];
item_person = [];
for i = 1:101
    mean(i) = sum(data(:,i))/101.0;
    for j = 1:24983
        if(data(i,j) == 0)
            item_person(i,j) = 0.4;
        else
            item_person(i,j) = data(i,j);
        end
    end
end

%svd ??may cost lots of time??
[u,s,v] = svd(item_person);  %u is the cluster matrix

%generate mean of the mark
average_item = [];
for i = 1:101
    average_item(i) = sum(item_person(i, :)) / 24983.0;
end

%calculate the predicted data
for i = 1:101
    for j = 1:24983
        item_person1(i,j) = item_person(i,j) - average_item(i);
    end
end

result = [];
result = u * item_person1;

final_result = [];
for i = 1:101
    for j = 1:24983
        final_result(i,j) = result(i,j) / sum(abs(u(i,:)));
        final_result(i,j) = final_result(i,j) + average_item(i);
    end
end

id_data = load('id_index.csv');

test_data = load('matrix_test_data.csv');
test_result_data = test_data;
%check the answer if exist already
for i = 1:101
    %mean = sum(data(:,i))/101.0;
    for j = 1:24983
        if(test_data(i,j) == 1 && item_person(i,j) ~= 0.4)
            test_result_data(i,j) = data(i,j);
        end
    end
end

%get the predicted answer
for i = 1:101
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
fid = fopen('svd_ver1.csv', 'w');
fprintf(fid, 'id,rating\n');
parfor i = 1:362091
    fprintf(fid, '%d,%g\n', [id_data(i) rdresult(i)]);
end
fclose(fid);

                
                


