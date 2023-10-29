#include <iostream>
#include <cmath>
using namespace std;

#define N (20)

class MyCordic
{
public:
    void init()
    {
        invSumK=1.;
        for(int i=0;i<N;i++)
        {
            Pow2[i]=pow(2.,-i);
            atanPow2[i]=atan(Pow2[i]);
            invSumK*=cos(atanPow2[i]);
            cout << "angle=" << Pow2[i] << " atan(angle)=" << atanPow2[i] << " invSumK=" << invSumK << endl;
        }
    }

    double compute(double beta)
    {
        double x=invSumK, y=0.;

        for(int i=0; i<N;i++)
        {
            if(beta<0.)
            { //rotation dans le sens trigo
                double xn=x+y*Pow2[i];
                y-=x*Pow2[i];
                beta+=atanPow2[i];
                x=xn;
            }
            else
            { //rotation dans le sens horaire
                double xn=x-y*Pow2[i];
                y+=x*Pow2[i];
                beta-=atanPow2[i];
                x=xn;
            }
            cout << "cos(beta)=" << x << " sin(beta)=" << y << " beta_reminder=" << beta << endl;
        }

        return x; //sin
    }

private:
    double Pow2[N];
    double atanPow2[N];
    double invSumK;
};

int main()
{
    MyCordic m;
    m.init();
    m.compute(1.2);
    return 0;
}
