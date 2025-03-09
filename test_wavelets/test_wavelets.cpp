#include <iostream>
#include <cassert>
#include <cmath>
#include <iomanip>
using namespace std;

#include "Wavelets.h"

///////////////////////////////////////////////////////////////////////////////
void print_vector(const vector<double>& a)
{
	for (int i = 0; i < a.size(); i++)
		cout << setprecision(2) << a[i] << " ";
	cout << endl;
}

int main()
{
	cout << "Testing Daubechie wavelelets transform/untransform" << endl;

	vector<double> data1, data2, data3, data4;
	Wavelets w(4);
	
	// init data1 with a ramp
	for (int i = 0; i < 8; i++)
		data1.push_back(0.);

	for (int i = 0; i < 8; i++)
		data1.push_back(i);

	for (int i = 0; i < 8; i++)
		data1.push_back(8.);

	cout << "original data:" << endl;
	print_vector(data1);

	cout << endl << "data wavelet transformed:" << endl;
	w.transform(data1, data2);
	print_vector(data2);

	cout << endl << "data wavelet untransformed:" << endl;
	w.untransform(data2, data3);
	print_vector(data3);

	// test with low pass coefs only
	cout << endl << "reconstructed from low pass coef:" << endl;
	for (int i = 12; i < 24; i++)
		data2[i]=0.; // remove high frequencies
	w.untransform(data2, data4);
	print_vector(data4);

    return 0;
}