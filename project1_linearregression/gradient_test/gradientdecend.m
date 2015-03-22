function [theta, J_history] = gradientdecend(X, y, theta, alpha, num_iters)

m = length(y); % number of training examples
J_history = zeros(num_iters, 1);
thetas = size(theta,1);
features = size(X,2)

mu = mean(X);
sigma = std(X);
mu_size = size(mu);
sigma_size = size(sigma);

%for all iterations
for iter = 1:num_iters

tempo = [];

result = [];

theta_temp = [];

%for all the thetas    
for t = 1:thetas
    %all the examples
    for examples = 1:m
       tempo(examples) = ((theta' * X(examples, :)') - y(examples)) * X(examples,t);
    end

    result(t) = sum(tempo);
    tempo = 0;

end

%theta temp, store the temp 
for c = 1:thetas

    theta = theta - alpha * (1/m) * result';

end

% Save the cost J in every iteration    
J_history(iter) = ComputeCost(X, y, theta);

end

theta
end
