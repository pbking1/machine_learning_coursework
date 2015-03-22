#include <iostream>
#include "armadillo"
using namespace std;
using namespace arma;

int main(){
	mat A(2, 3);
	cout<<A.n_rows<<endl;
	cout<<A.n_cols<<endl;
	A(1, 2) = 456.;
	A.print("A: ");
	A.set_size(3, 3);
	A.fill(5.0);
	A.print("A: ");
	mat B,C;
	 B << 0.555950 << 0.274690 << 0.540605 << 0.798938 << endr
		     << 0.108929 << 0.830123 << 0.891726 << 0.895283 << endr
			     << 0.948014 << 0.973234 << 0.216504 << 0.883152 << endr
				     << 0.023787 << 0.675382 << 0.231751 << 0.450332 << endr;
	B.print("B: ");
	cout<<"B: "<<endl<<B<<endl;
//	B.save("~/Document/machine_learning/class_project/project1_linearregression/normaleqn_project1_ver1/c++_test/B.txt", raw_ascii);
	C.load("ex1data2.txt");
	//C.print("C: ");
	mat D;
	D = C.submat(0, 0, 1, 1);
	D.print("D: ");
	D = eye<mat>(4,4);
	D.print("D: ");
	return 0;
}
