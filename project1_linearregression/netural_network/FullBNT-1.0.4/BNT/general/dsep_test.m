
% Cowell et al p72
G = zeros(10);
G(1,2)=1;
G(2,3)=1;
G(3,7)=1;
G(4,[5 8])=1;
G(5,6)=1;
G(6,7)=1;
G(7,[9 10])=1;
G(8,9)=1;

dsep(1, 4, [5 7], G)
dsep(1, 4, [7], G)
dsep(1, 4, [10 5], G)
