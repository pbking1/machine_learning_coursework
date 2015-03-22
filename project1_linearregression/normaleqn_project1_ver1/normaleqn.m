function [theta] = normaleqn(x, y, w)
    theta = zeros(size(x, 2), 1);
    %theta = pinv(x' * x + 4000.3 * eye(size(x, 2))) * x' * y;
     %theta = pinv(x' * x + 3.3 * eye(size(x, 2))) * x' * y;
    theta = pinv(x' * x + w * eye(size(x, 2))) * x' * y;
end
