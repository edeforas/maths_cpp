// Programme de Compression des images par Ondelettes
// Par Rania ELDAHDAH, Etienne DEFORAS et Morgan ROCHE
// version maj du 10/06/2023 : EDF


#include "Wavelets.h"

#include <cmath>
using namespace std;

#define MAX (256)

int taille=MAX*MAX;

////////////////////////////////////////////////////////////////////////////////
Wavelets::Wavelets()
{
    initcoef(4);
}

////////////////////////////////////////////////////////////////////////////////
void Wavelets::copy(const double source[256][256], double cible[256][256])
{
    for (int ligne = 0; ligne < 256; ligne++)
        for (int colonne = 0; colonne < 256; colonne++)
            cible[colonne][ligne] = source[colonne][ligne];
}
////////////////////////////////////////////////////////////////////////////////
void Wavelets::zero(double cible[256][256],int xmin, int xmax, int ymin, int ymax, double erreur)
{
    for (int ligne = ymin; ligne < ymax; ligne++)
        for (int colonne = xmin; colonne < xmax; colonne++)
            if (fabs(cible[colonne][ligne]) < erreur)
                cible[colonne][ligne] = 0.;
}

////////////////////////////////////////////////////////////////////////////////
void Wavelets::initcoef(int n)
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

    _ncof = n;

    if (n == 2)
    {
        _paire = haar;
        _impaire = haarr;
    }
    else if (n == 4)
    {
        _paire = c4;
        _impaire = c4r;
    }
    else if (n == 12)
    {
        _paire = c12;
        _impaire = c12r;
    }
    else if (n == 20)
    {
        _paire = c20;
        _impaire = c20r;
    }

    double sig = -1.0;
    for (int k = 0; k < n; k++)
    {
        _impaire[n - k - 1] = sig * _paire[k];
        sig = -sig;
    }
}
////////////////////////////////////////////////////////////////////////////////
void Wavelets::transform_wavelet_1d(int nn,double a[256])
{
    double www[MAX];
    double wwwi, wwwinh;
    int k, n = nn;
    for (int ligne = 0; ligne < (n >> 1); ligne++)
    {
        wwwinh = wwwi = 0;
        for (k = 0; k < _ncof; k++)
        {
            double temp = a[(k + ligne + ligne) % n];
            wwwi += _paire[k] * temp;
            wwwinh += _impaire[k] * temp;
        }
        www[ligne + (n >> 1)] = wwwinh;
        www[ligne] = wwwi;
    }
    for (k = 0; k < n; k++)
        a[k] = www[k];
}
////////////////////////////////////////////////////////////////////////////////
void Wavelets::transform_wavelet_2d(double img[256][256],double ond1[256][256])
{
    double a[256];
    copy(img, ond1);
    for (int x = MAX; x >= 4; x >>= 1)
    {
        for (int colonne = 0; colonne < x; colonne++)
        {
            for (int ligne = 0; ligne < x; ligne++)
                a[ligne] = ond1[colonne][ligne];

            transform_wavelet_1d(x,a);
            for (int ligne = 0; ligne < x; ligne++)
                ond1[colonne][ligne] = a[ligne];
        }
        for (int colonne = 0; colonne < x; colonne++)
        {
            for (int ligne = 0; ligne < x; ligne++)
                a[ligne] = ond1[ligne][colonne];

            transform_wavelet_1d(x,a);
            for (int ligne = 0; ligne < x; ligne++)
                ond1[ligne][colonne] = a[ligne];
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void Wavelets::untransform_wavelet_1d(int n, double img[256])
{
    double www[MAX],a[256];

    for (int k = 0; k < n; k++)
        www[k] = 0;

    for (int col = 0; col < (n >> 1); col++)
    {
        double ai = a[col];
        double ai1 = a[(col + (n >> 1)) % n];
        for (int k = 0; k < _ncof; k++)
        {
            www[((col << 1) + k) % n] += _paire[k] * ai;
            www[((col << 1) + k) % n] += _impaire[k] * ai1;
        }
    }

    for (int k = 0; k < n; k++)
        a[k] = www[k];
}
////////////////////////////////////////////////////////////////////////////////
void Wavelets::untransform_wavelet_2d(double ond2[256][256], double fic[256][256])
{
    double a[256];
    copy(ond2, fic);
    for (int x = 4; x <= MAX; x <<= 1)
    {
        for (int colonne = 0; colonne < x; colonne++)
        {
            for (int ligne = 0; ligne < x; ligne++)
                a[ligne] = fic[colonne][ligne];

            untransform_wavelet_1d(x,a);

            for (int ligne = 0; ligne < x; ligne++)
                fic[colonne][ligne] = a[ligne];
        }
        for (int colonne = 0; colonne < x; colonne++)
        {
            for (int ligne = 0; ligne < x; ligne++)
                a[ligne] = fic[ligne][colonne];

            untransform_wavelet_1d(x,a);

            for (int ligne = 0; ligne < x; ligne++)
                fic[ligne][colonne] = a[ligne];
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
double Wavelets::compute_error(double ond2[256][256],int xmin, int xmax, int ymin, int ymax, double pourcent)
{
    double vmin = 0., vmax = 0., vmoy, test = 0, nbinf = 0., nbtot = 0.;
    for (int ligne = ymin; ligne < ymax; ligne++)
        for (int colonne = xmin; colonne < xmax; colonne++)
            if (ond2[colonne][ligne] > vmax)
                vmax = ond2[colonne][ligne];

    vmoy = (vmin + vmax) / 2.;
    while ((test < (pourcent - 1.)) || (test > (pourcent + 1.)))
    {
        if (vmin == vmax)
            return(-1.);
        for (int ligne = ymin; ligne < ymax; ligne++)
            for (int colonne = xmin; colonne < xmax; colonne++)
            {
                if (ond2[colonne][ligne] < vmoy) nbinf++;
                nbtot++;
            }
        test = nbinf / nbtot * 100.;

        if (test > pourcent)
        {
            vmax = vmoy;
            vmoy = (vmin + vmoy) / 2.;
            nbinf = 0.;
            nbtot = 0.;
        }
        if (test < pourcent)
        {
            vmin = vmoy;
            vmoy = (vmax + vmoy) / 2.;
            nbinf = 0.;
            nbtot = 0.;
        }
        if (vmax < vmin + .5) break;
    }
    return vmoy;
}
////////////////////////////////////////////////////////////////////////////////
double Wavelets::compare(double tab1[256][256], double tab2[256][256])
{
    double errquad = 0.;
    for (int j = 0; j < 256; j++)
        for (int i = 0; i < 256; i++)
        {
            double temp = (int)(tab1[i][j] + .5) - (int)(tab2[i][j] + .5);
            errquad += (temp * temp);
        }

    return errquad / (MAX*MAX);
}
////////////////////////////////////////////////////////////////////////////////
void Wavelets::compress(double ond1[256][256], double ond2[256][256])
{
    int xmin = 0, xmax = 4;
    double pourcent = 100., erreur, ecart = .1, esp;

    esp = 1.;
    while (esp > ecart)
    {
        erreur = compute_error(xmin, xmax, xmin, xmax, pourcent);
        zero(xmin, xmax, xmin, xmax, erreur);
        untransform_wavelet_2d(ond1,ond2);
        esp = compare(img, fic) * taille / (xmax * xmax);
        if (esp > ecart)
        {
            pourcent -= 10.;
            copy(ond1, ond2);
        }
        if (!pourcent)
        {
            copy(ond1, ond2);
            break;
        }
    }

    copy(ond2, ond1);
    for (xmin = 4; xmin < MAX; xmin <<= 1)
    {
        xmax = xmin * 2;
        pourcent = 100.;

        esp = 1.;
        while (esp > ecart)
        {
            erreur = compute_error(xmin, xmax, 0, xmax, pourcent);
            zero(xmin, xmax, 0, xmax, erreur);
            untransform_wavelet_2d(ond2,fic);
            esp = compare(img, fic) * taille / ((xmax - xmin) * (xmax - xmin));
            if (esp > ecart)
            {
                pourcent -= 10.;
                copy(ond1, ond2);
            }
            if (!pourcent)
            {
                copy(ond1, ond2);
                break;
            }
        }
        copy(ond2, ond1);
        pourcent = 100.;

        esp = 1.;
        while (esp > ecart)
        {
            erreur = compute_error(0, xmax, xmin, xmax, pourcent);
            zero(0, xmax, xmin, xmax, erreur);
            untransform_wavelet_2d(ond2,fic);
            esp = compare(img, fic) * taille / ((xmax - xmin) * (xmax - xmin));
            if (esp > ecart)
            {
                pourcent -= 10.;
                copy(ond1, ond2);
            }
            if (!pourcent)
            {
                copy(ond1, ond2);
                break;
            }
        }
        copy(ond2, ond1);
        pourcent = 100.;

        esp = 1.;
        while (esp > ecart)
        {
            erreur = compute_error(xmin, xmax, xmin, xmax, pourcent);
            zero(xmin, xmax, xmin, xmax, erreur);
            untransform_wavelet_2d();
            esp = compare(img, fic) * taille / ((xmax - xmin) * (xmax - xmin));
            if (esp > ecart)
            {
                pourcent -= 10.;
                copy(ond1, ond2);
            }
            if (!pourcent)
            {
                copy(ond1, ond2);
                break;
            }
        }
        copy(ond2, ond1);
    }
}
////////////////////////////////////////////////////////////////////////////////