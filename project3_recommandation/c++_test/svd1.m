data = load('matrix_data.csv');

%??????????
%1.????????????????????????
%2.??????????????????????????????????????
average_item = [];
for i = 1:101
    s = 0;
    n = 0;
    for j = 1:24983
        if(data(i,j) ~= 0)
            s = s + data(i,j);
            n = n +1;
        end
    end
    average_item(i) = s/n;
end

average_item(1) = 0;

item_person = [];
for i = 1:101
    for j = 1:24983
        if(data(i,j) == 0)
            item_person(i,j) = average_item(i);
        else
            item_person(i,j) = data(i,j);
        end
    end
end

%??????
test= zscore(item_person);

%svd????
%[U,S,V] = svd(test');
U = load('svd_u.csv');
S = load('svd_s.csv');
V = load('svd_v.csv');

%??S??S??????????????????????????????,????????????????????????????11??????29??
%S??????????????0????????????S=29*29????????U=100*29?? V=29*101
%????test'=100*101=U*S*V=100*29   *    29*29    *29*101 
u = U(:,1:15);
v = V(:,1:15);
s = S(1:15,1:15);

test = test';

%??????=R(i)????+U*sqrt(s)(i) *  sqrt(s)*V(j)
news = sqrt(s);
newu = u * news;
newv = v * news;
test_final = [];
for i = 1:24983
    for j = 1:101
        new1 = newv(j,:)'; 
        test_final(i,j) = mean(test(i,:))+newu(i,:)*new1;
    end
end

%????????????
test_ftran = test_final';
real_final = [];
add = item_person(1:101,1:24983);
avera = mean(add);
sd = std(add,0);
for i = 1:101
    for j = 1:24983
        real_final(i,j) = test_ftran(i,j) * sd(j) + avera(j);
    end
end
        
id_data = load('id_index.csv');

test_data = load('matrix_test_data.csv');
test_result_data = test_data;
%check the answer if exist already
for i = 1:101
    for j = 1:24983
        if(test_data(i,j) == 1 && item_person(i,j) ~= average_item(i))
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
fid = fopen('svdver4.csv', 'w');
fprintf(fid, 'id,rating\n');
for i = 1:362091
    fprintf(fid, '%d,%g\n', [id_data(i) rdresult(i)]);
end
fclose(fid);



