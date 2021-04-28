#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <stdio.h>
using namespace std;
//////////////////////////////////////////////////////////////////////////////
void random_sieve()
{
    cout << "method: random_sieve ( disp every 1000)" << endl;
    int iCircle=0;
    int iSquare=0;

    for (int i=0;i<100000;i++)
    {
        double dx=rand()/(double)RAND_MAX;
        double dy=rand()/(double)RAND_MAX;

        iSquare++;
        if (dx*dx+dy*dy<1.)
            iCircle++;

        if (i%1000==0)
        {
            cout << setprecision(15) << i/1000 << " : " << 4.*iCircle/iSquare << endl;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////
void euler_1()
{
    cout << "method: euler_1" << endl;
    double s=1.;
    for (int i=1;i<50;i++)
    {
        double a=1.;
        double b=1.;

        for (int j=1;j<=i;j++)
        {
            a=a*j;
            b=b*(2*j+1);
        }

        s+=a/b;

        cout << setprecision(15) << i << " : " << 2.*s << endl;
    }
}
//////////////////////////////////////////////////////////////////////////////
void euler_2()
{
    cout << "method: euler_2" << endl;
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
//from "Le merveilleux nombre PI"
void compte_goutte_1()
{
    int a=10000,b=0,c=8400,d,e=0,f[8401],g;
    for ( ;b-c ;)
        f[b++]=a/5;
    for(;d=0,g=c*2;c-=14,printf("%.4d",e+d/a),e=d%a)
        for(b=c ;d+=f[b]*a,f[b]=d%--g,d/=g--,--b;d*=b)
        ;
}
//////////////////////////////////////////////////////////////////////////////
//from "Le merveilleux nombre PI"
void compte_goutte_2() //WIP
{
    int a=10000,b=0,c=8400,e=0,f[8401],g;

    while (b!=c)
        f[b++]=a/5;

    for (c=8400;c>0;c-=14)
    {
        g=c*2;
        int d=0;
        b=c;

        for ( ;d+=f[b]*a,f[b]=d%--g,d/=g--,--b; )
            d*=b;

        printf("%.4d",e+d/a);
        e=d%a;
    }
}
//////////////////////////////////////////////////////////////////////////////

int main()
{
    //random_sieve();
    //euler_1();
    euler_2();
    //compte_goutte_1();
    //compte_goutte_2();
    return 0;
}
