%pre = ones(40000,1);
fid = fopen('final.csv', 'w');
fprintf(fid, 'Id,label\n');
for i = 1:40000
    fprintf(fid, '%g,%g\n', i, aa(i) );
end
fclose(fid);