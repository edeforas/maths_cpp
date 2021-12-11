#include <iostream>
#include <cmath>
using namespace std;

#define SIZE  100
#define MAX_ORDER_TAB 10

typedef double Filter_Coef[5][MAX_ORDER_TAB];
typedef double Memory_Coef[2][MAX_ORDER_TAB];

#define PI (3.1415926535897932384626433832795) 

//public domain?

/**************************************************************
*        Program to demonstrate the Butterworth filter        *
*           (removing frequencies greater than Fc)            *
*                                                             *
*       C++ version with graphic results by J-P Moreau, Paris *
*                                                             */

/**********************************************************************
*          Filtering a signal F(t) by Butterworth method              *
*             (removing frequencies greater then Fc)                  *
* ------------------------------------------------------------------- *
* Calling mode:   Filter(Xs,Xd,NSections,C,D);                        *
* ------------------------------------------------------------------- *
* INPUTS:                                                             *
* -------                                                             *
*        Xd.......:  current value of input signal (REAL)             *
*        NSections: Number of required 2nd order sections (integer)   *
*                   = n/2     for n even                              *
*                   = (n+1)/2 for n odd                               *
*        n........: order of filter (1 to 4)                          *
*        C........: Table[1..5,1..NSections] of filter coefficients   *
*                   calculated previously by BUTTERWORTH procedure    *
*        D........: Table[1..2,1..NSections] of coefficients defining *
*                   the filter memory, initialized by INIT procedure. *
* ------------------------------------------------------------------- *
* OUTPUTS:                                                            *
* -------                                                             *
*        D........: Table updated after the call to Filter procedure  *
*        Xs.......: current value of filtered signal (REAL)           *
* ------------------------------------------------------------------- *
* Reference                                                           *
* ---------                                                           *
*  "Lawrence R.Rabiner et Bernard Gold                                *
*   Theory and application of digital processing.                     *
*   Prentice Hall Inc., EnglewoodclIFfs,NEW JERSEY,1975."             *
*                                                                     *
*                                   C++ version by J-P Moreau, Paris  *
*               from Fortran version by J-P Dumont / Tuan Dang Trong  *
**********************************************************************/
double Filter(double Xd, int NSections,Filter_Coef C, Memory_Coef D )
{
    double x,y,err;
    x=Xd;
    for (int i=0; i<NSections; i++)
    {
        err=x+C[0][i]*D[0][i]+C[1][i]*D[1][i];
        y=C[4][i]*(err +C[2][i]*D[0][i]+C[3][i]*D[1][i]);
        D[1][i]=D[0][i];
        D[0][i]=err;
        x=y;
    }
    return x;
}

/*************************************************************************
*                       INIT FILTER PROCEDURE                            *
* ---------------------------------------------------------------------- *
* The filter response is initialized to stationnary value for a constant *
* input signal value.                                                    *
*                                                                        *
* Calling mode:   INIT(Xdc,C,NSections,D);                               *
* ---------------------------------------------------------------------- *
* INPUTS:                                                                *
* ------                                                                 *
*        Xdc......: constant input value (REAL)                          *
*        C........: Table[1..5,1..NSections] of filter coefficients      *
*                   calculated previously by BUTTERWORTH procedure       *
*        NSections: Number of required 2nd order sections (integer)      *
*                   = n/2     for n even                                 *
*                   = (n+1)/2 for n odd                                  *
*        n........: order of filter (1 to 4)                             *
* ---------------------------------------------------------------------- *
* OUTPUTS:                                                               *
* -------                                                                *
*        D........: Table[1..2,1..NSections] of coefficients defining    *
*                   the filter memory, initialized by INIT procedure.    *
*************************************************************************/
void Init(double Xdc, Filter_Coef C, int NSections, Memory_Coef D)
{
    double dc,Csum;
    int i,j;
    dc=Xdc;
    for (j=0; j<NSections; j++)
    {
        D[1][j]=dc/(1.-C[0][j]-C[1][j]);
        D[0][j]=D[1][j];
        Csum=0.;
        for (i=0; i<4; i++)
            Csum=Csum + C[i][j];

        dc=C[4][j]*(dc+D[1][j]*Csum);
    }
}

/*********************************************************************
*          Calculates the Butterworth filter coefficients            *
* ------------------------------------------------------------------ *
*  Calling mode:   Butterworth(Fc,Ts,n,C,NSections,Tg);              *
* ------------------------------------------------------------------ *
*  INPUTS:                                                           *
*  ------                                                            *
*         Fc.......: Cut off frequency                               *
*         Ts.......: Sampling time of input signal                   *
*         n........: Order of filter                                 *
* ------------------------------------------------------------------ *
*  OUTPUTS:                                                          *
*  -------                                                           *
*         C........: Table[1..5,1..NSections] of filter coefficients *
*                    calculated previously by BUTTERWORTH procedure  *
*         NSections: Number of required 2nd order sections (integer) *
*                    = n/2     for n even                            *
*                    = (n+1)/2 for n odd                             *
*         Tg.......: Group delay in seconds                          *
*********************************************************************/
void Butterworth(double Fc,double Ts,int iOrder,Filter_Coef C,int *NSections, double *Tg )
{
    int Ns2,Modn;
    double Arg,Rep,Omega,OmegaSq,temp,W0,W1,m;

    for (int i=0; i<MAX_ORDER_TAB; i++)
    {
       C[0][i]=0.;
       C[1][i]=0.;
       C[2][i]=0.;
       C[3][i]=0.;
       C[4][i]=0.;
    }

    Arg= PI *Ts*Fc;
    if (fabs(Arg) > 2.0* PI)
    {
        m=int(Arg/2.0/ PI);
        Arg=Arg-(m*2.0* PI);
    }
    Omega= tan(Arg);
    OmegaSq=Omega*Omega;
    Modn=(iOrder % 2);
    if (Modn==0)
        temp=0.5;
    else
        temp=0.;

    Ns2=iOrder/2;
    *NSections=Ns2+Modn;
    *Tg=0.;
    if (iOrder>1)
        for (int i=0; i<Ns2; i++)
        {
            Rep=Omega*cos(PI *(i+1-temp)/iOrder);
            *Tg=*Tg+Ts*Rep/OmegaSq;
            W0=2.*Rep;
            W1=1. +W0+OmegaSq;
            C[0][i]=-2.*(OmegaSq-1.)/W1;
            C[1][i]=-(1.-W0+OmegaSq)/W1;
            C[2][i]=2.;
            C[3][i]=1.;
            C[4][i]=OmegaSq/W1;
        }

    if (temp == 0.)
    {
        C[0][*NSections-1]=(1.-Omega)/(1.+Omega);
        C[1][*NSections-1]= 0.;
        C[2][*NSections-1]= 1.;
        C[3][*NSections-1]= 0.;
        C[4][*NSections-1]= Omega/(1.+Omega);
        *Tg= *Tg+Ts/(2.*Omega);
    }
}
///////////////////////////////////////////////////////////////////////////////
int main()
{
    double  signal[SIZE],filtered[SIZE];

    Filter_Coef C;
    Memory_Coef D;
    int    NSections;
    double   Tg;

    double dSamplingTime=1./100.;
    double dCutoffFrequency=5.;
    int iOrder=1;

    // 1. Calculate the filter coefficients
    Butterworth(dCutoffFrequency,dSamplingTime,iOrder,C,&NSections,&Tg);

    // 2. Initialize filter memory
    Init(0.,C,NSections,D);

    // create step signal
    for (int i=0;i<SIZE/2.;i++)
        signal[i]=0.;
    for (int i=int(SIZE/2.);i<SIZE;i++)
        signal[i]=1.;

    // 3. Recursively call Butterworth filter
    for (int i=0; i<SIZE;i++)
        filtered[i]=Filter(signal[i],NSections,C,D);

    //print input and filtered signals to output file
    cout << "signal:\tfiltered:" << endl;
    for (int i=0; i<SIZE; i++)
        cout << "\t" << signal[i] << "\t" << filtered[i] << endl;

    cout << endl << "Buttwerworth parameters: NSections=" << NSections << endl;
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<MAX_ORDER_TAB;j++)
            cout << C[i][j] << "\t" ;
        cout << endl;
    }

    return 0;
}
