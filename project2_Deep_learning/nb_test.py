import csv
from numpy import genfromtxt
my_data = genfromtxt('train.csv', delimiter=',')
X = my_data[:, 2:3074]
Y = my_data[:, 1]
data = genfromtxt('test.csv', delimiter=',')
x = data[:, 1:]

from sklearn.naive_bayes import GaussianNB
from sklearn import datasets
gnb = GaussianNB()
gnb = gnb.fit(X, Y)
result = gnb.predict(x) 
#result[0] is the label

import numpy
numpy.savetxt("nb_output.csv", result, delimiter=",")
