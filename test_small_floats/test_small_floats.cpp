#include <iostream>
#include <string>
#include <cmath>
using namespace std;

#include "SmallFloats.h"

// choose your real number implementation below:
//typedef float real;
//typedef double real;
//typedef long double real;
//typedef RealDouble real; // same as double but in a class
//typedef RealFloat real; // same as float but in a class
//typedef Float8q8 real;
//typedef Fixpt8q8 real;
typedef FixptTanh8 real;
int main()
{
	cout << "sizeof(real)=" << sizeof(real) << endl;

	// newton step to sqrt(2) to test accuracy
	real x = 2.f;
	real u = 1.f;
	for (int i = 0; i < 10; i++)
	{
		u = real(0.5f)*(u + real(x) / u);
		cout << "i=" << i << " x=" << x << " u("<< i <<")=" << u << " u(" << i<< ")**2=" << u * u << endl;
	}

	return 0;
}