#include <iostream>
#include <cassert>
#include <cmath>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
float fast_sqrt_init(float val)
{
    union
    {
        int tmp;
        float val;
    } u;
    u.val = val;
    u.tmp -= 1 << 23; // Remove last bit so 1.0 gives 1.0
    // tmp is now an approximation to logbase2(val)
    u.tmp >>= 1; // divide by 2
    u.tmp += 1 << 29; // add 64 to exponent: (e+127)/2 =(e/2)+63,
    // that represents (e/2)-64 but we want e/2
    return u.val;
}
///////////////////////////////////////////////////////////////////////////////
float fast_invsqrt_init(float x)
{
    //first part of invsqrt_quake3
    union
    {
        float x;
        int i;
    } u;
    u.x = x;
    u.i = 0x5f3759df - (u.i >> 1);
    return u.x;
}
/////////////////////////////////////////////////////////////////////
double sqrt_wolfram(double a)
{
    double u=a/64.; //to keep 1<=u<4
    double v=0.;
    int N=16;
    double dfactor = 8. / (1LL << (N + 1));

    for(int i=0;i<N;i++)
    {
        if(u>v)
        {
            u=4.*(u-v-1.);
            v=2.*(v+2.);
        }
        else
        {
            u=4.*u;
            v=2.*v;
        }
    }

    return v * dfactor;
}
/////////////////////////////////////////////////////////////////////
double sqrt_bhaskara_brouncker(double a)
{
    double x = fast_sqrt_init(a);
    double y= 1;

    for(int i=0;i<30;i++)
    {
        double oldx=x;
        x=x+y*a;
        y=oldx+y;
    }

    return x/y;
}
/////////////////////////////////////////////////////////////////////
double sqrt_wilkes(double a)
{
    double pre_factor = 10.;
    a=a/(pre_factor* pre_factor); //must have 0<a<3

    //cf http://en.wikipedia.org/wiki/Methods_of_computing_square_roots
    double a0=a;
    double c0=a-1.;

    for(int i=0;i<10;i++)
    {
        a0=a0-a0*c0/2.;
        c0=c0*c0*(c0-3.)/4.;
    }

    return a0 * pre_factor;
}
/////////////////////////////////////////////////////////////////////
double sqrt_mathpath(double a)
{
    double x =  fast_sqrt_init(a);
    for(int i=0;i<20;i++)
        x=(x+a)/(x+1.);

    return x;
}
/////////////////////////////////////////////////////////////////////
unsigned int sqrt_int_approx(unsigned int a)
{
    // author is Etienne de Foras, 
    // mix of a bit count algorithm using SWAR and two Newton cycles
    unsigned int r,v=a;
    unsigned int shift;

    r = (v > 0xFFFF) << 4;
    v >>= r;
    shift = (v > 0xFF ) << 3;
    v >>= shift;
    r |= shift;
    shift = (v > 0xF ) << 2;
    v >>= shift;
    r |= shift;
    shift = (v > 0x3 ) << 1;
    v >>= shift;
    r |= shift;
    r |= (v >> 1);

    r= 1 << (r >> 1);
    r=(r+a/r)>>1;
    r=(r+a/r)>>1;

    return r;
}
/////////////////////////////////////////////////////////////////////
//	Square root by abacus algorithm, Martin Guy @ UKC, June 1985.From a book on programming abaci by Mr C. Woo.
//	Argument is a positive integer, as is result. formally proved that on exit:
//		   2		   2		   2		res  <= x < (res+1)	and	res  + op == x
//	This is also nine times faster than the library routine (-lm).
int sqrt_abacus(int x)
{
    //	Logically, these are unsigned. We need the sign bit to test whether (op - res - one) underflowed.
    int op=x, res=0, one= 1 << 30; // second-to-top bit set

    // "one" starts at the highest power of four <= than the argument.
    while (one > op) 
        one >>= 2;

    while (one != 0)
    {
        if (op >= res + one)
        {
            op = op - (res + one);
            res = res +  (one << 1);
        }
        res >>=1;
        one >>=2;
    }
    return res;
}
///////////////////////////////////////////////////////////////////////////////
float invsqrt_quake3(float x)
{
    union
    {
        float x;
        int i;
    } u;
    u.x = x;
    u.i = 0x5f3759df - (u.i >> 1);
    x = u.x * (1.5f - 0.5f*x * u.x * u.x);
    return x;
}

///////////////////////////////////////////////////////////////////////////////
float goldschmidt_invsqrt(float x)
{
    // from https://en.wikipedia.org/wiki/Methods_of_computing_square_roots
    float y0 = fast_invsqrt_init(x);
    float xn = y0 * x;
    float hn = y0 * 0.5f;

    for (int i = 0; i < 3; i++)
    {
        float rn = 0.5f - xn * hn;
        xn = xn + xn * rn; //FMA can be simplified using rn=rn+1 and factorized
        hn = hn + hn * rn;
    }

    //return xn; // xn converge to sqrt(x) 
    return 2.f * hn; // yn converge to invsqrt(x)
}

///////////////////////////////////////////////////////////////////////////////
double sqrt_halley(double a)
{
    double u= fast_sqrt_init(a);

    for (int i=0;i<3;i++)
    {
        u=u*(u*u+3.*a)/(3.*u*u+a);
    }
    return u;
}
///////////////////////////////////////////////////////////////////////////////
double sqrt_newton_div(double a)
{
    double u= fast_sqrt_init(a);

    for (int i=0;i<3;i++)
        u=1/2.*(u+a/u);

    return u;
}
///////////////////////////////////////////////////////////////////////////////
double inv_sqrt_newton_mult(double a)
{
    double v= fast_invsqrt_init(a);

    for (int i=0;i<3;i++)
        v=v*0.5*(3.-a*v*v);

    return v*a;
}
///////////////////////////////////////////////////////////////////////////////
int main()
{
    double a=100.;

    cout << "a=" << a << " ref_sqrt=" << sqrt(a) << endl << endl;

    cout << "fast_sqrt_init:" << fast_sqrt_init((float)a) << endl;
    cout << "fast_invsqrt_init:" << a * fast_invsqrt_init((float)a) << endl <<endl;

    cout << "sqrt_newton_div:" << sqrt_newton_div(a) << endl;
    cout << "sqrt_halley:" << sqrt_halley(a) << endl;
    cout << "sqrt_wilkes:" << sqrt_wilkes(a) << endl;
    cout << "sqrt_mathpath:" << sqrt_mathpath((float)a) << endl;
    cout << "sqrt_bhaskara_brouncker:" << sqrt_bhaskara_brouncker(a) << endl;
    cout << "sqrt_wolfram:" << sqrt_wolfram((float)a) << endl <<endl;

    cout << "a*inv_sqrt_newton_mult:" << inv_sqrt_newton_mult(a) << endl;
    cout << "a*invsqrt_quake3:" << a*invsqrt_quake3((float)a) << endl;
    cout << "a*goldschmidt_invsqrt:" << a * goldschmidt_invsqrt((float)a) << endl <<endl;

    cout << "sqrt_int_approx:" << sqrt_int_approx((int)a) << endl;
    cout << "abacus:" << sqrt_abacus((int)a) << endl;

    return 0;
}
