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

	// test 32 bit bmp write: black and white Sierpinsky fractal
	Image img(512, 512, 4);
	for(int w=0;w<img.width();w++)
		for (int h = 0; h < img.height(); h++)
			*(int*)(img.pixel(w, h)) = ((w & h)==0) * ((1<<25)-1); //Sierpinsky
	ImageIoBmp::write("sierpinsky.bmp",&img);

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
