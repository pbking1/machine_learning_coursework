import csv
from numpy import genfromtxt
my_data = genfromtxt('train.csv', delimiter=',')
X = my_data[:, 3:3074]
Y = my_data[:, 2]
data = genfromtxt('test.csv', delimiter=',')
x = data[:, 2:3073]

from sklearn.svm import LinearSVC
clf2 = LinearSVC()
clf2.fit(X, Y)
result = clf2.predict(x)

import numpy
numpy.savetxt("lsvm_output.csv", result, delimiter=",")
