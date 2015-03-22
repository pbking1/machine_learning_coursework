function [theta, J_history] = gd(feature, y_value, theta, alpha, iter_number)

m = length(y_value);
J_history = zeros(iter_number, 1);
for iter = 1:iter_number
    Update = 0;
    for i = 1:m
        Update = Update + alpha / m * ( theta' * feature(i, :)'  - y_value(i)) * feature(i, :)';
    end
    theta = theta - Update;
    J_history(iter) = ComputeCost(feature, y_value, theta);
end