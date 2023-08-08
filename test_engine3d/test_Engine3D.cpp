#include <cstdlib>
#include <cmath>

#include "Engine3D.h"
#include "Image.h"
#include "ImageIoBmp.h"

#define PIXELRGB(a,b,c) ((a<<16)+(b<<8)+c)

////////////////////////////////////////////////////////////////////////////////
void draw(Image& img)
{
	FACET f123, f124, f135, f236;
	P3D p1, p2, p3, p4, p5, p6;

	int xMax = img.width(), yMax = img.height();
	double dAngleX = 0., dAngleY = 0., dAhead = 100.;

	Engine3D eng(xMax, yMax, (int*)img.data());

	// init facets
	p1.ax = 1000; 		p1.ay = 1000; 		p1.az = 0;
	p2.ax = 0; 			p2.ay = 1000; 		p2.az = 1000;
	p3.ax = 1000; 		p3.ay = 0; 		p3.az = 1000;
	p4.ax = 0; 			p4.ay = 1000; 	  	p4.az = 0;
	p5.ax = 1000; 		p5.ay = 0; 		p5.az = 0;
	p6.ax = 0; 			p6.ay = 0; 		p6.az = 1000;
	f123.a = &p1;  	f123.b = &p2; 	f123.c = &p3;
	f123.tex = 0;
	f123.atx = 0; 		f123.aty = 0;
	f123.btx = 255 * 256; f123.bty = 0;
	f123.ctx = 0;		f123.cty = 255 * 256;
	f124.a = &p1;  	f124.b = &p2; 	f124.c = &p4;
	f124.tex = 0;
	f124.atx = 10; 		f124.aty = 0;
	f124.btx = 13 * 256; 	f124.bty = 0;
	f124.ctx = 14 * 256; 		f124.cty = 13 * 256;
	f135.a = &p1;  	f135.b = &p3; 	f135.c = &p5;
	f135.tex = 0;
	f135.atx = 3 * 256; 		f135.aty = 128 * 256;
	f135.btx = 21 * 256; 	f135.bty = 17 * 256;
	f135.ctx = 32 * 256; 	f135.cty = 15 * 256;
	f236.a = &p2;  	f236.b = &p3; 	f236.c = &p6;
	f236.tex = 0;
	f236.atx = 4 * 256; 		f236.aty = 0;
	f236.btx = 8 * 256; 	f236.bty = 17 * 256;
	f236.ctx = 7 * 256; 	f236.cty = 21 * 256;

	eng.LoadTex();
	eng.set_camera(0., 0., 0., dAhead, dAngleX, dAngleY, 0, 1000.);

	Point3D a;
	srand(0);
	for (int point = 0; point < 20000; point++)
	{
		a.x = (rand() % 1000) / 100.;
		a.y = (rand() % 1000) / 100.;
		a.z = (rand() % 1000) / 100.;

		int colorr = point * 30000;

		eng.draw_pixel(a, colorr);
	}

	//trace un cube autour du nuage de points
	eng.draw_line(Point3D(10, 10, -10), Point3D(10, 10, 10), PIXELRGB(255, 255, 255));
	eng.draw_line(Point3D(10, -10, -10), Point3D(10, -10, 10), PIXELRGB(255, 255, 255));
	eng.draw_line(Point3D(-10, 10, -10), Point3D(-10, 10, 10), PIXELRGB(255, 255, 255));
	eng.draw_line(Point3D(-10, -10, -10), Point3D(-10, -10, 10), PIXELRGB(255, 255, 255));

	eng.draw_line(Point3D(10, -10, 10), Point3D(10, 10, 10), PIXELRGB(255, 255, 255));
	eng.draw_line(Point3D(-10, -10, 10), Point3D(-10, 10, 10), PIXELRGB(255, 255, 255));
	eng.draw_line(Point3D(10, -10, -10), Point3D(10, 10, -10), PIXELRGB(255, 255, 255));
	eng.draw_line(Point3D(-10, -10, -10), Point3D(-10, 10, -10), PIXELRGB(255, 255, 255));

	//	eng.draw_trianglecolor(Point3D(-10,10,-10),PIXELRGB(0,0,255),Point3D(10,-10,-10),PIXELRGB(0,255,0), Point3D(10,10,-10),PIXELRGB(255,0,0) );

	eng.draw_facet(f123);
	eng.draw_facet(f124);
	eng.draw_facet(f135);
	eng.draw_facet(f236);
}
/////////////////////////////////////////////////////////////////////////////
int main()
{
	Image img(512, 512, 4);
	draw(img);
	ImageIoBmp::write("textured.bmp", &img);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////