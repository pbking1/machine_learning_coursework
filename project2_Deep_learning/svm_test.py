import csv
from numpy import genfromtxt
my_data = genfromtxt('trainaa', delimiter=',')
X = my_data[:, 3:3074]
Y = my_data[:, 2]
data = genfromtxt('testaa', delimiter=',')
x = data[:, 2:3073]

from sklearn import svm
clf2 = svm.SVC()
clf2.fit(X,Y)
result = clf2.predict(x)

import numpy
numpy.savetxt("svm_output.csv", result, delimiter=",")
