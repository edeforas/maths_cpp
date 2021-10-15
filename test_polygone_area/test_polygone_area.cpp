// compute the 2D polygon signed area using :
// https://en.wikipedia.org/wiki/Shoelace_formula

#include <iostream>
using namespace std;

void main()
{
    double x[] = { 3,5,12,9,5,3 }; //last point is duplicated from first point
    double y[] = { 4,11,8,5,6,4 }; //last point is duplicated from first point

    //compute signed area
	double s=0.;
    for (int i=0; i<size(x)-1; i++)
        s+= x[i]*y[i+1]-x[i+1]*y[i];

    cout << "Polygon Signed Area = " << s/2. << endl;
}