%data = load('matrix_data.csv');
%matlabpool local 2

%replace the unjudged data
item_person = [];
for i = 1:101
    for j = 1:24983
        if(data(i,j) == 0)
            item_person(i,j) = -0.4;
        else
            item_person(i,j) = data(i,j);
        end
    end
end

%calculate the similarity
for i = 1:101
    for j = 1:101
        if(i == j)
            cos_val(i,j) = 1;
        else
            cos_val(i,j) = sum((item_person(i,:)) .* (item_person(j,:)));
            v1 = sqrt(sum((item_person(i,:)) * (item_person(i,:)')));
            v2 = sqrt(sum((item_person(j,:)) * (item_person(j,:)')));
            cos_val(i,j) = cos_val(i,j) / (v1 * v2);
        end
    end
end

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

%for i = 1:101
 %   for j = 1:24983
  %      item_person1(i,j) = item_person1(i,j) - mean(i);
  %  end
%end

result = cos_val * item_person1;

final_result = [];
for i = 1:101
    for j = 1:24983
        final_result(i,j) = result(i,j) / sum(abs(cos_val(i,:)));
        final_result(i,j) = final_result(i,j) + average_item(i);
    end
end

%id_data = load('id_index.csv');

%test_data = load('matrix_test_data.csv');
test_result_data = test_data;
%check the answer if exist already
for i = 1:101
    %mean = sum(data(:,i))/101.0;
    for j = 1:24983
        if(test_data(i,j) == 1 && item_person(i,j) ~= -0.4)
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
fid = fopen('mean_ver3.csv', 'w');
fprintf(fid, 'id,rating\n');
for i = 1:362091
    fprintf(fid, '%d,%g\n', [id_data(i) rdresult(i)]);
end
fclose(fid);


%matlabpool close;
                
                


