data = load('matrix_data.csv');

%replace the unjudged data
%generate mean of the mark
sum = [];
average_item = [];
for i = 1:101
    s = 0;
    n = 0;
    for j = 1:24983
        if(item_person(i,j) ~= 0)
            s = s + item_person(i,j);
            n = n +1;
        end
    end
    average_item(i) = s/n;
end

item_person = [];
for i = 1:101
    %mean = sum(data(:,i))/101.0;
    for j = 1:24983
        if(data(i,j) == 0)
            item_person(i,j) = average_item(i);
        else
            item_person(i,j) = data(i,j);
        end
    end
end

%calculate the similarity
%????????????matlab????????corrcoef(x)??x??????????????????????
s = corrcoef(item_person')

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

real_final = [];
for i = 1:101
    for j = 1:24983
        real_final(i,j) = result(i,j) / sum(abs(cos_val(i,:)));
        real_final(i,j) = real_final(i,j) + average_item(i);
    end
end




