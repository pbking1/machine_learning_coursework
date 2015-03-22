function [X_norm, mu, sigma] = featureNormal(X)

X_norm = X;
mu = zeros(1, size(X, 2));
sigma = zeros(1, size(X, 2));

mu = mean(X);
sigma = std(X);

for i = 1:size(X_norm,2)
    X_norm(:,i) = (X(:,i) - mu(i)) / sigma(i);  %error
end

end

