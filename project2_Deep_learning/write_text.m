fid = fopen('final_v1.csv', 'w');
fprintf(fid, 'Id,label\n');
for i = 1:40000
    fprintf(fid, '%g,%g\n', i, round(final1(i)));
end
fclose(fid);