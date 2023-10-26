#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cstdio>
#include <cmath>
using namespace std;

//////////////////////////////////////////////////////////////////////////////
void random_sieve()
{
    cout << "method: random_sieve ( disp every 100000)" << endl;
    int iInCircle=0;
    int iSquare=0;

    for (int i=0;i<10000000;i++)
    {
        double dx=rand()/(double)RAND_MAX;
        double dy=rand()/(double)RAND_MAX;

        iSquare++;
        if (dx*dx+dy*dy<1.)
            iInCircle++;

        if (i%100000==0)
        {
            cout << setprecision(15) << i/100000 << " : " << 4.*iInCircle/iSquare << endl;
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
        b=b*(2*i+1.);
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
void polygon()
{
    // d is the inner polygon edge size (start with a diameter =2.)
    //split the edge in two parts, move middle point to circle, recompute half edge size
    
    cout << "method: polygon" << endl;
    double u, v, d =2.;
    double nb_edge = 1.;
    for (int i = 0; i < 50; i++)
    {
        u = d * d / 4.;
        v = 1. - sqrt(1. - u);
        d = sqrt(u + v*v);
        nb_edge = nb_edge * 2.;
         
        cout << setprecision(15) << i << " : " << d* nb_edge << endl;
    }
}
//////////////////////////////////////////////////////////////////////////////
void machin1()
{
    //use pi = 4 * ATAN(1)
    double dEstPi = 0.;
    double dMinusOne = 1.;
    for (int i = 0; i < 1000; i++)
    {
        dEstPi += 4. * dMinusOne * (1. / (2. * i + 1.));

        cout << setprecision(15) << "EstPi=" << dEstPi << endl;
        dMinusOne = - dMinusOne;
    }
}
//////////////////////////////////////////////////////////////////////////////
void machin2()
{
    //use pi=4*ATAN(1/2)+4*atan(1/3)
    double dEstPi = 0.;
    double dMinusOne = 1.;

    double k2=1./2.;
    double k3 =1./3.;

    for (int i = 0; i < 10; i++)
    {
        dEstPi += 4. * dMinusOne * (k2 / (2. * i + 1.));
        dEstPi += 4. * dMinusOne * (k3 / (2. * i + 1.));

        cout << setprecision(15) << "EstPi=" << dEstPi << endl;
        dMinusOne = -dMinusOne;

        k2 *= (1. / 2.) * (1. / 2.);
        k3 *= (1. / 3.) * (1. / 3.);
    }
}
//////////////////////////////////////////////////////////////////////////////
void machin3()
{
    // use pi=16*ATAN(1/5)-4*atan(1/239)
    double dEstPi = 0.;
    double dMinusOne = 1.;

    double k2 = 1. / 5.;
    double k3 = 1. / 239.;

    for (int i = 0; i < 10; i++)
    {
        dEstPi += 16. * dMinusOne * (k2 / (2. * i + 1.));
        dEstPi -= 4. * dMinusOne * (k3 / (2. * i + 1.));

        cout << setprecision(15) << "EstPi=" << dEstPi << endl;
        dMinusOne = -dMinusOne;

        k2 *= (1. / 5.) * (1. / 5.);
        k3 *= (1. / 239.) * (1. / 239.);
    }
}
//////////////////////////////////////////////////////////////////////////////
int main()
{
    //random_sieve();
    //euler();
    //compte_goutte();
    //polygon();
    machin3();
    return 0;
}
