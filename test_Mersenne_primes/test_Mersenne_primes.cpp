#include <iostream>
using namespace std;

bool is_prime_trivial(int p)
{
    int iRang=2;
    while (iRang*iRang<=p)
    {
        if ( (p % iRang)==0)
            return false;

        iRang++;
    }
    return true;
}

bool is_odd(int p)
{
    return (p%2)!=0;
}

bool is_mersenne_prime(int iQ)
{
    int Mq=(1<<iQ)-1;
    int Sn=4;
    for (int i=2;i<iQ;i++)
    {
        Sn=Sn*Sn-2;
        Sn=Sn % Mq;
    }

    if (Sn==0)
    {
        cout << "q= " << iQ << "  Sn= " << Sn << "  Mq= " << Mq << " is prime!" << endl;
        return true;
    }
    else
    {
        cout << "q= " << iQ << "  Sn= " << Sn << "  Mq= " << Mq << " is not prime." << endl;
        return false;
    }
}

int main()
{
    for(int i=1;i<100;i++)
        cout << "i=" << i << " is_prime_trivial=" << is_prime_trivial(i) << endl;

    for (int i=1;i<14;i++)
        if (is_odd(i))
            if (is_prime_trivial(i))
                is_mersenne_prime(i);

    return 0;
}
