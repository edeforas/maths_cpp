#include <iostream>
#include <cassert>
#include <cmath>
using namespace std;

#include "Wavelets.h"

///////////////////////////////////////////////////////////////////////////////
void print(const vector<double> a)
{
	//todo
}

int main()
{
	vector<double> data1,data2, data3;
	Wavelets w;
	
	// init data1

	print(data1);

	w.transform(data1, data2);
	print(data2);

	w.untransform(data2, data3);
	print(data3);

    return 0;
}
