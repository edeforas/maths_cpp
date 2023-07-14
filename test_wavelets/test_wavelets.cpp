// Programme de Compression des images par Ondelettes
// Par Rania ELDAHDAH, Etienne DEFORAS et Morgan ROCHE
// version maj du 10/06/2023 : EDF

#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#define MAX (256)

#include "Image.h"
#include "ImageIoBmp.h"
#include "Wavelets.h"

////////////////////////////////////////////////////////////////////////////////
int main()
{

    Wavelets w;

    // load image
    Image im;
    ImageIoBmp imw;
    imw.read("fractal_orig.bmp", &im);

    // todo

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
