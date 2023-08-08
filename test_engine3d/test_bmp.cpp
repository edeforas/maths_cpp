#include <iostream>
using namespace std;

#include "Image.h"
#include "ImageIoBmp.h"

////////////////////////////////////////////////////////////////////////////////
int main()
{
	// test image I/O
	Image im;
	if (ImageIoBmp::read("mandelbrot.bmp", &im) == false)
	{
		cout << "Unable to read mandelbrot.bmp, good path ?" << endl;
		exit(-1);
	}

	if (ImageIoBmp::write("mandelbrot2.bmp", &im) == false)
	{
		cout << "Unable to write mandelbrot2.bmp, good path and rights?" << endl;
		exit(-1);
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
