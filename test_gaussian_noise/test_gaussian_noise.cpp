#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <cmath>
using namespace std;

double uniform()
{
    return ((double)rand()/RAND_MAX*2.-1.);
}

double gaussian()
{
    int inbconv=10;
    double d=0.;
    for (int i=0;i<inbconv;i++)
    {
        d+=uniform();
    }
    return d * sqrt(3.) /sqrt((double)inbconv);
}

int main()
{
    cout << "Gaussian number generation using uniform law" << endl;

    vector<double> histo(20);

    int nbtest=100000;
    double eqt=0.;
    for (int i=0;i<nbtest;i++)
    {
        double d=gaussian();
        eqt+=d*d;

        histo[(int)d+10]++;
    }
    cout << "histogram=" << endl;
    for (int i = 0; i < 20; i++)
    {
        cout << setprecision(3) << histo[i] / nbtest << " ";
    }

    cout << endl << "Stddev=" << sqrt(eqt/nbtest) << endl;

    return 0;
}
