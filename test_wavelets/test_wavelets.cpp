// Programme de Compression des images par Ondelettes
// Par Rania ELDAHDAH, Etienne DEFORAS et Morgan ROCHE
// version maj du 10/06/2023 : EDF

#include <fstream>
#include <iostream>
using namespace std;

#define MAX (256)

#include "Image.h"
#include "ImageIoBmp.h"
#include "wavelets.h"

////////////////////////////////////////////////////////////////////////////////
void load_img(double img[MAX][MAX])
{
    Image im;
    ImageIoBmp imw;
    imw.read("fractal_orig.bmp", &im);

    for (int ligne = 0; ligne < im.width(); ligne++)
        for (int colonne = 0; colonne < im.height(); colonne++)
            img[colonne][ligne]=im(ligne, colonne);
}
/////////////////////////////////////////////////////////////////////////////////
void save_compressed(double image[256][256])
{
    Image im(MAX, MAX);

    for (int ligne = 0; ligne < MAX; ligne++)
        for (int colonne = 0; colonne < MAX; colonne++)
            im(ligne, colonne) = (unsigned char)(image[colonne][ligne]+0.5);

    ImageIoBmp imw;
    imw.write("fractal_compressed.bmp", &im);
}
////////////////////////////////////////////////////////////////////////////////
void save_csv(double image[256][256])
{
	ofstream fp("fractal_wavelet.csv");
    for (int ligne=0;ligne<MAX;ligne++)
	{
		for (int colonne=0;colonne<MAX;colonne++)
        {
            double u= image[colonne][ligne];
            if (fabs(u)<1.)
				u=0.;
            fp << u << " ";
        }
		fp << endl;
	}
}
////////////////////////////////////////////////////////////////////////////////
int main()
{
    double orig[MAX][MAX];
    double ond1[MAX][MAX];
    double ond1_compress[MAX][MAX];
    double out_img[MAX][MAX];

    Wavelets w;

    load_img(orig);
    w.transform_wavelet_2d(orig,ond1);
    w.compress(ond1,ond1_compress);
    w.untransform_wavelet_2d(ond1_compress,out_img);
	save_csv(ond1_compress);
    save_compressed(out_img);
    double dError= w.compare(orig,out_img);
	cout <<  "Compression Stddev: " << dError << endl;

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
