fid = fopen('ssa.csv', 'w');
fprintf(fid, 'Id,reference\n');
for i = 1:10000
    fprintf(fid, '%g,%g\n', i + 10000, aaa(i));
end
fclose(fid);