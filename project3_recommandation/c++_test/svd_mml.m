data = load('matrix_data.csv');

%??????????
%1.????????????????????????
%2.??????????????????????????????????????
average_item = [];
for i = 1:101
    average_item(i) = sum(data(i, :)) / 24983.0;
end

item_person = [];
for i = 1:101
    for j = 1:24983
        if(data(i,j) == 0)
            item_person(i,j) = -0.2;
        else
            item_person(i,j) = data(i,j);
        end
    end
end

%??????
test= zscore(item_person);

%svd????
[U,S,V] = svd(test');

%??S??S??????????????????????????????,????????????????????????????11??????29??
%S??????????????0????????????S=29*29????????U=100*29?? V=29*101
%????test'=100*101=U*S*V=100*29   *    29*29    *29*101 
u = U(:,1:29);
v = V(1:29,:);
s = S(1:29,1:29);

%??????=R(i)????+U*sqrt(s)(i) *  sqrt(s)*V(j)
news = sqrt(s);
newu = u * news;
newv = news * v;
test_final = [];
for i = 1:100
    for j = 1:101
        test_final(i,j) = mean(test(i,:))+newu(i,:)*newv(:,j);
    end
end

%????????????
test_ftran = test_final';
real_final = [];
add = item_person(1:101,1:100);
avera = mean(add);
sd = std(add,0);
for i = 1:101
    for j = 1:100
        real_final(i,j) = test_ftran(i,j) * sd(j) + avera(j);
    end
end
        



