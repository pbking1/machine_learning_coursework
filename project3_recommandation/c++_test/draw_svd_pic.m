temps = [];
tempindex = [];
for i = 1:101
    temps(i) = S(i,i);
    tempindex(i) = i;
end

plot(tempindex, temps);
%draw the pic of the S matrix and find the break point(??)