// Programme de Compression des images par Ondelettes
// Par Rania ELDAHDAH, Etienne DEFORAS et Morgan ROCHE
// version maj du 10/06/2023 : EDF

#include "Wavelets.h"

#include <cmath>

////////////////////////////////////////////////////////////////////////////////
Wavelets::Wavelets(int nCoef)
{
    initcoef(nCoef);
}
////////////////////////////////////////////////////////////////////////////////
void Wavelets::initcoef(int nCoef)
{
    //Daubechie
    static double c4[4] = { 0.4829629131445341,0.8365163037378077,0.2241438680420134,-0.1294095225512603 };
    static double c12[12] = { 0.1115407433501095,0.4946238903984533,0.751133908021059,0.3152503517091982,
                            -0.2262646939654400,-0.1297668675672625,0.0975016055873225,0.0275228655303053,
                            -0.0315820393174862,0.0005538422011614,0.0047772575109455,-0.0010773010853085
    };
    static double c20[20] = { 0.0266700579005473,0.1881768000776347,0.5272011889315757,0.6884590394534363,
                            0.2811723436605715,-0.2498464243271598,-0.1959462743772862,0.1273693403357541,
                            0.0930573646035547,-0.0713941471663501,-0.0294575368218399,0.0332126740593612,
                            0.0036065535669870,-0.0107331754833007,0.0013953517470699,0.0019924052951925,
                            -0.0006858566949564,-0.0001164668551285,0.0000935886703202,-0.0000132642028945
    };

    //Haar
    static double haar[2] = { 0.7071067811865475, 0.7071067811865475 };

    static double c4r[4], c12r[12], c20r[20], haarr[2];

    _nCoef = nCoef;

    if (nCoef == 2)
    {
        _paire = haar;
        _impaire = haarr;
    }
    else if (nCoef == 4)
    {
        _paire = c4;
        _impaire = c4r;
    }
    else if (nCoef == 12)
    {
        _paire = c12;
        _impaire = c12r;
    }
    else if (nCoef == 20)
    {
        _paire = c20;
        _impaire = c20r;
    }

    double sig = -1.0;
    for (int k = 0; k < nCoef; k++)
    {
        _impaire[nCoef - k - 1] = sig * _paire[k];
        sig = -sig;
    }
}
////////////////////////////////////////////////////////////////////////////////
void Wavelets::transform(const vector<double>& a, vector<double>& b)
{
    double wwwi, wwwinh;
    int n = a.size();
    for (int l = 0; l < (n >> 1); l++)
    {
        wwwinh = wwwi = 0.;
        for (int k = 0; k < _nCoef; k++)
        {
            double temp = a[(k + l + l) % n];
            wwwi += _paire[k] * temp;
            wwwinh += _impaire[k] * temp;
        }
        b[l + (n >> 1)] = wwwinh;
        b[l] = wwwi;
    }
}
////////////////////////////////////////////////////////////////////////////////
void Wavelets::untransform(const vector<double>& a, vector<double>& b)
{
    int n = a.size();
    for (int k = 0; k < n; k++)
        b[k] = 0.;

    for (int col = 0; col < (n >> 1); col++)
    {
        double ai = a[col];
        double ai1 = a[(col + (n >> 1)) % n];
        for (int k = 0; k < _nCoef; k++)
        {
            b[((col << 1) + k) % n] += _paire[k] * ai;
            b[((col << 1) + k) % n] += _impaire[k] * ai1;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////