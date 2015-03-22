import csv
from numpy import genfromtxt
my_data = genfromtxt('train.csv', delimiter=',')
X = my_data[:, 2:3074]
Y = my_data[:, 1]
data = genfromtxt('test.csv', delimiter=',')
x = data[:, 1:]

from sklearn.linear_model import LogisticRegression
clf2 = LogisticRegression().fit(X, Y)
result = clf2.predict_proba(x)

import numpy
numpy.savetxt("lr_output.csv", result, delimiter=",")
