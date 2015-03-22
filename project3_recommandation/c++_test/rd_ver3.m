data = load('matrix_data.csv');

%replace the unjudged data
item_person = [];
average= mean(data');
for i = 1:101
    %mean = sum(data(:,i))/101.0;
    for j = 1:24983
        if(data(i,j) == 0)
            %item_person(i,j) = -0.2;
            item_person(i,j) = average(i);
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
            cos_val(i,j) = sum(item_person(i,:) .* item_person(j,:));
            v1 = sqrt(sum(item_person(i,:) * item_person(i,:)'));
            v2 = sqrt(sum(item_person(j,:) * item_person(j,:)'));
            cos_val(i,j) = cos_val(i,j) / (v1 * v2);
        end
    end
end

%generate mean of the mark

average_item = [];
for i = 1:101
    average_item(i) = sum(item_person(i, :)) / 24983.0;
end

for i = 1:101
    for j = 1:24983
        item_person1(i,j) = item_person(i,j) - average_item(i);
    end
end

%generate the mark( by the items whose |similarity|>0.1)
cos_val1 = [];
for i = 1:101
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
for i = 1:101
    for j = 1:24983
        final_result(i,j) = result(i,j) / sum(abs(cos_val(i,:)));
        final_result(i,j) = final_result(i,j) + average_item(i);
    end
end




