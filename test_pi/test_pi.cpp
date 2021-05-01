#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <stdio.h>
using namespace std;
//////////////////////////////////////////////////////////////////////////////
void random_sieve()
{
    cout << "method: random_sieve ( disp every 100000)" << endl;
    int iCircle=0;
    int iSquare=0;

    for (int i=0;i<10000000;i++)
    {
        double dx=rand()/(double)RAND_MAX;
        double dy=rand()/(double)RAND_MAX;

        iSquare++;
        if (dx*dx+dy*dy<1.)
            iCircle++;

        if (i%100000==0)
        {
            cout << setprecision(15) << i/100000 << " : " << 4.*iCircle/iSquare << endl;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////
void euler()
{
    cout << "method: euler" << endl;
    double s=1.;
    double a=1.;
    double b=1.;

    for (int i=1;i<50;i++)
    {
        a=a*i;
        b=b*(2*i+1);
        s+=a/b;

        cout << setprecision(15) << i << " : " << 2.*s << endl;
    }
}
//////////////////////////////////////////////////////////////////////////////
//from the book "Le merveilleux nombre PI" J.P. Delahaye
void compte_goutte()
{
    int a=10000,b=0,c=8400,d,e=0,f[8401],g;
    for ( ;b-c ;)
        f[b++]=a/5;
    for(;d=0,g=c*2;c-=14,printf("%.4d",e+d/a),e=d%a)
        for(b=c ;d+=f[b]*a,f[b]=d%--g,d/=g--,--b;d*=b)
        ;
}
//////////////////////////////////////////////////////////////////////////////
void archimed()
{
    cout << "method: archimed" << endl;
    double x = 0. , d ;
    double factor2 = 2.;
    for (int i = 0; i < 20; i++)
    {
        x = sqrt((1.+x)*0.5);
        d = 2.*factor2*sqrt(2. - 2. * x);
        cout << setprecision(15) << i << " : " << d << endl;

        factor2 = factor2 * 2.;
    }
}
//////////////////////////////////////////////////////////////////////////////
int main()
{
    //random_sieve();
    //euler();
    //compte_goutte();
    archimed();
    return 0;
}
