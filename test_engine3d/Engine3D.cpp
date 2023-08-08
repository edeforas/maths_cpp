#include <cmath>
using namespace std;

#include "Engine3D.h"

//very old code

#define MAXINT (1<<28)

// used by mapfacet
#define my_swap(a,b,t) { t=a; a=b; b=t; }
#define MINZ  (100)

////////////////////////////////////////////////////////////////////////////////
void Engine3D::set_camera(double ox, double oy, double oz, double ahead, double yaw, double pitch, double roll, double zoom)
{
	_camera.set_origin(ox, oy, oz, ahead);
	_camera.set_angles(yaw, pitch, roll);
	_camera.set_screen(_Xmax, _Ymax, zoom);
}
////////////////////////////////////////////////////////////////////////////////
Engine3D::Engine3D(int xm, int ym, int* pBuffer)
{
	_Xmax = xm;
	_Ymax = ym;
	_pBuffer = pBuffer;

	_zbuffer = new float[_Xmax * _Ymax];
	_zbuffer16 = new float[_Xmax * _Ymax / 16];
	_zbuffer256 = new float[_Xmax * _Ymax / 256];

	tex = 0;

	clear();
}
////////////////////////////////////////////////////////////////////////////////
Engine3D::~Engine3D()
{
	delete[] tex;
	delete[] _zbuffer;
	delete[] _zbuffer16;
	delete[] _zbuffer256;
}
////////////////////////////////////////////////////////////////////////////////
void Engine3D::draw_trianglecolor(Point3D& A, int pA, Point3D& B, int pB, Point3D& C, int pC)
{
	Point3D E;
	double t;

	//tester si la facette est a l'envers

	//tester la visibilite dans l'ecran

	//echanger les points suivant leur ordre en y

	//separer en deux triangles( trapezes de bases horizontales )

	//couper chaque trapeze suivant la visibilité dans l'ecran ( en y )

	//tracer trapeze 1
	//pour chaque ligne, tester la visibilité en x ( couper si besoins)
	//puis tracer dans le mzbuffer

	//tracer trapeze 2
	//pour chaque ligne, tester la visibilité en x ( couper si besoins)
	//puis tracer dans le mzbuffer

	for (t = 0; t < 1; t += 0.05)
	{
		E.x = t * A.x + (1. - t) * B.x;
		E.y = t * A.y + (1. - t) * B.y;
		E.z = t * A.z + (1. - t) * B.z;
		draw_line(E, C, pC);
	}

	for (t = 0; t < 1; t += 0.05)
	{
		E.x = t * B.x + (1. - t) * C.x;
		E.y = t * B.y + (1. - t) * C.y;
		E.z = t * B.z + (1. - t) * C.z;
		draw_line(E, A, pA);
	}

	for (t = 0; t < 1; t += 0.05)
	{
		E.x = t * C.x + (1. - t) * A.x;
		E.y = t * C.y + (1. - t) * A.y;
		E.z = t * C.z + (1. - t) * A.z;
		draw_line(E, B, pB);
	}
}
////////////////////////////////////////////////////////////////////////////////
void Engine3D::clear()
{
	for (int j = 0; j < _Ymax; j++)
	{
		int* decall = j * _Xmax + _pBuffer;
		float* decz = j * _Xmax + _zbuffer;
		for (int i = 0; i < _Xmax; i++)
		{
			decall[i] = 0;    //le zbuffer est en 1/z > a l'infini =0.!
			decz[i] = 0.;
		}
	}

	//todo clear zbuffer
}
////////////////////////////////////////////////////////////////////////////////
void Engine3D::draw_line(Point3D& p1, Point3D& p2, int col)
{
	//todo optimize
	int x1, y1, x2, y2, decal;
	double fx, fy, dx, dy;
	int i, im;
	float zp1, zp2, fz, dz;

	_camera.project(p1, x1, y1, zp1);
	_camera.project(p2, x2, y2, zp2);

	dx = x2 - x1;
	dy = y2 - y1;
	if (fabs(dx) > fabs(dy))
		im = int(fabs(dx));
	else
		im = int(fabs(dy));

	if (im < 1)
		return;

	fx = x1;
	dx = dx / im;
	fy = y1;
	dy = dy / im;
	fz = zp1;
	dz = (zp2 - zp1) / im;

	for (i = 0; i < im; i++)
	{
		if ((fx >= 0) && (fx < _Xmax) && (fy >= 0) && (fy < _Ymax))// && (fz>0) )
		{
			decal = int(fx) + int(fy) * _Xmax;

			if (_zbuffer[decal] < fz)
			{
				_zbuffer[decal] = fz;
				_pBuffer[decal] = col;
			}
		}
		fx += dx;
		fy += dy;
		fz += dz;
	}
}
////////////////////////////////////////////////////////////////////////////////
void Engine3D::draw_pixel(Point3D& pPixels, int col)
{
	int ix, iy;
	float zpx;
	float* zp;

	if (!_camera.project(pPixels, ix, iy, zpx))
		return;

	//teste if visible in zbuffer
	zp = _zbuffer + ix + iy * _Xmax;

	//on compare par rapport au pixel dans le zbuffer
	// et on met a jour le zbuffer
	if ((*zp) < zpx)
	{
		*zp = zpx;
		_pBuffer[ix + iy * _Xmax] = col;
	}
}
////////////////////////////////////////////////////////////////////////////////
void Engine3D::draw_facet(FACET f)
{
	P3D pa, pb, pc, tm;
	int pb_a_gauche, t, * pta, * ptb, * ptc, * ptm;

	pa = *f.a;
	pb = *f.b;
	pc = *f.c;

	pta = &f.atx;
	ptb = &f.btx;
	ptc = &f.ctx;

	if (pc.ey < pb.ey)
	{
		my_swap(pb, pc, tm);    //on ordonne les points en ey croissant :a,b,c
		my_swap(ptb, ptc, ptm);
	}
	if (pb.ey < pa.ey)
	{
		my_swap(pb, pa, tm);
		my_swap(ptb, pta, ptm);
	}
	if (pc.ey < pb.ey)
	{
		my_swap(pb, pc, tm);
		my_swap(ptb, ptc, ptm);
	}

	if (pc.ey < 0) return;
	if (pa.ey > _Ymax) return;

	ystart = pa.ey;
	int ymin1 = pa.ey;
	int ymin2 = ymin1;
	yend = pb.ey;

	if (yend > _Ymax)
		yend = _Ymax;

	gsx = dsx = pa.nx;
	gsy = dsy = pa.ny;
	gsz = dsz = pa.nz;

	gex = pb.nx;
	gey = pb.ny;
	gez = pb.nz;
	dex = pc.nx;
	dey = pc.ny;
	dez = pc.nz;

	a1 = pb.ex - pa.ex;
	a2 = pc.ex - pa.ex;
	b1 = pb.ey - pa.ey;
	b2 = pc.ey - pa.ey;
	a1x1 = pa.ex * b1 + (b1 >> 1);
	a2x2 = pa.ex * b2 + (b2 >> 1);

	pb_a_gauche = ((b1 * a2 - b2 * a1) >= 0);
	if (!pb_a_gauche) //le mapping s'arrete  apres b1  (premier point:b)
	{
		my_swap(gex, dex, t);
		my_swap(gey, dey, t);
		my_swap(gez, dez, t);

		my_swap(a1x1, a2x2, t);  //on inverse les x pour avancer dans la memoire en adresse croissante
		my_swap(a1, a2, t);
		my_swap(b1, b2, t);
		my_swap(ptb, ptc, ptm);
	}

	gstx = dstx = *pta;
	gsty = dsty = *(pta + 1); //texture du point a

	getx = *ptb;     //texture du point b
	gety = *(ptb + 1);

	detx = *ptc;     //texture du point c
	dety = *(ptc + 1);

	if (yend < 0)     //ne pas tracer la 1ere trapez
	{
		a1x1 += (yend - ystart) * a1;     //mais avancer aux point final pour la 2eme trapez
		a2x2 += (yend - ystart) * a2;
	}
	else
		MapTrapez();

	ystart = pb.ey; //yend;    //deuxieme partie du triangle
	yend = pc.ey;
	if (ystart > _Ymax) return;      //hors de l'ecran
	if (yend > _Ymax) yend = _Ymax;

	if (pb_a_gauche)
	{
		dex = gex = pc.nx;
		dey = gey = pc.ny;
		dez = gez = pc.nz;

		gsx = pb.nx;
		gsy = pb.ny;
		gsz = pb.nz;

		ymin1 = pb.ey;
		a1 = pc.ex - pb.ex;
		b1 = pc.ey - pb.ey;
		a1x1 = pb.ex * b1 + (b1 >> 1);
		gstx = *ptb;
		gsty = *(ptb + 1);
		detx = getx = *ptc;
		dety = gety = *(ptc + 1);

	}
	else
	{
		dex = gex = pc.nx;
		dey = gey = pc.ny;
		dez = gez = pc.nz;

		dsx = pb.nx;
		dsy = pb.ny;
		dsz = pb.nz;

		ymin2 = pb.ey;
		a2 = pc.ex - pb.ex;
		b2 = pc.ey - pb.ey;
		a2x2 = pb.ex * b2 + (b2 >> 1);
		dstx = *ptc;
		dsty = *(ptc + 1);

		detx = getx = *ptb;
		dety = gety = *(ptb + 1);

	}

	MapTrapez();
	(void)ymin2;
}
////////////////////////////////////////////////////////////////////////////////
void Engine3D::MapTrapez()
{
	int j, xg, xd;

	if (!(b1 && b2)) return;

	if ((ystart) < 0)
	{
		a1x1 += -a1 * (ystart);
		a2x2 += -a2 * (ystart);
		ystart = 0;
	}

	for (j = ystart; j < yend; j++)
	{
		xg = a1x1 / b1;
		xd = a2x2 / b2;

		CalcI12txy(j);

		lineH(xg, j, xd);
		a1x1 += a1;
		a2x2 += a2;
	}
}
////////////////////////////////////////////////////////////////////////////////
void Engine3D::CalcI12txy(int j)
{
	float t1, t2, t1d, t1n, t2d, t2n;

	t1n = gsy - gsz * (j - _Ymax / 2.);
	t1d = (j - _Ymax / 2.) * (gez - gsz) - (gey - gsy);

	if (t1d != 0)
		t1 = t1n / t1d;
	else
		t1 = 0.;

	t2n = (float)(dsy - dsz * (j - _Ymax / 2));
	t2d = (float)((j - _Ymax / 2) * (dez - dsz) - (dey - dsy));

	if (t2d != 0)
		t2 = t2n / t2d;
	else
		t2 = 0.;

	i1tx = round(gstx + t1 * (getx - gstx));
	i1ty = round(gsty + t1 * (gety - gsty));

	i2tx = round(dstx + t2 * (detx - dstx));
	i2ty = round(dsty + t2 * (dety - dsty));

	i1x = round(gsx + t1 * (gex - gsx));
	i1y = round(gsy + t1 * (gey - gsy));
	i1z = round(gsz + t1 * (gez - gsz));

	i2x = round(dsx + t2 * (dex - dsx));
	i2y = round(dsy + t2 * (dey - dsy));
	i2z = round(dsz + t2 * (dez - dsz));
}
////////////////////////////////////////////////////////////////////////////////
void Engine3D::lineH(int x1, int y, int x2) // todo optimize
{
	int x, x116;

	if (x2 == x1) return;

	if (x2 < 0) return;
	if (x1 > _Xmax) return;
	if (x2 > _Xmax) x2 = _Xmax;

	if (x1 < 0)
	{
		x1 = 0;
		calctxy(x1);
		atx = itx;
		aty = ity;
		az = iz;
	}
	else
	{
		atx = i1tx;
		aty = i1ty;
		az = i1z;
	}

	pPixels = _pBuffer + x1 + y * _Xmax;

	x116 = ((x1 + 15) & ~15);
	if (x116 > x2) x116 = x2;
	calctxy(x116);

	btx = itx;
	bty = ity;
	bz = iz;

	if ((az > MINZ) && (bz > MINZ) && (x2 != x1))
	{
		invz = MAXINT / az;
		dinvz = (MAXINT / bz - invz) / (x2 - x1);

		line15(x1, y, x116);
	}

	for (x = x116 + 16; x <= x2; x += 16)
	{
		calctxy(x);
		atx = btx;
		aty = bty;
		az = bz;
		btx = itx;
		bty = ity;
		bz = iz;

		if ((bz > MINZ) && (az > MINZ))
		{
			dinvz = (MAXINT / bz - invz) / 16;
			line16(x - 16, y);
		}
	}

	if (x2 == _Xmax)
	{
		calctxy(_Xmax);
		btx = itx;
		bty = ity;
		bz = iz;
	}
	else
	{
		btx = i2tx;
		bty = i2ty;
		bz = i2z;
	}

	if (x - 16 < x2)
	{
		if ((bz > MINZ) && (az > MINZ) && (x1 != x2))
		{
			dinvz = (MAXINT / bz - invz) / 16;
			line15(x - 16, y, x2);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void Engine3D::line15(int x1, int y, int x2)
{
	int x, c, dtx, dty;

	if (x2 == x1) return;

	dtx = round((btx - atx) / (x2 - x1));
	dty = round((bty - aty) / (x2 - x1));

	for (x = x1; x < x2; x++)
	{
		if (invz > _zbuffer[x + y * _Xmax])
		{
			_zbuffer[x + y * _Xmax] = invz;
			c = ((atx >> 8) + (aty & ~255)) & 65535;
			*pPixels = tex[c]; //todo rounding
		}
		pPixels++;
		atx += dtx;
		aty += dty;
		invz += dinvz;
	}
}
////////////////////////////////////////////////////////////////////////////////
void Engine3D::line16(int x1, int y)
{
	int x, c, dtx, dty;

	dtx = round((btx - atx) / 16);
	dty = round((bty - aty) / 16);

	for (x = x1; x < x1 + 16; x++)
	{
		if (invz > _zbuffer[x + y * _Xmax])
		{
			_zbuffer[x + y * _Xmax] = invz;
			c = ((atx >> 8) + (aty & ~255)) & 65535;

			*pPixels = tex[c]; //was dectex
		}
		pPixels++;
		atx += dtx;
		aty += dty;
		invz += dinvz;
	}
}
////////////////////////////////////////////////////////////////////////////////
void Engine3D::LoadTex()   //temporary
{
	tex = new int[256 * 256];

	int i, j;
	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			tex[i + j * 256] = (i & j);//+(i<<5)+(j<<11);
}
////////////////////////////////////////////////////////////////////////////////
void Engine3D::calctxy(int x)
{
	float t, td, tn;

	tn = (float)(i1x - i1z * (x - _Xmax / 2));
	td = (float)((x - _Xmax / 2) * (i2z - i1z) - (i2x - i1x));
	if (td != 0.)
		t = tn / td;
	else
		t = 1.;

	itx = round(i1tx + t * (i2tx - i1tx));
	ity = round(i1ty + t * (i2ty - i1ty));
	iz = round(i1z + t * (i2z - i1z));
}
////////////////////////////////////////////////////////////////////////////////
