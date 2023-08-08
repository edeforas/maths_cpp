#ifndef _Engine3D_
#define _Engine3D_

//very old code

class P3D
{
public:
	int ax, ay, az, nx, ny, nz, ex, ey;
	int adr;
};

// textured triangle definition
class FACET
{
public:
	int norx, nory, norz, cnorx, cnory, cnorz; //normals
	int centrx, centry, centrz, dist;  //distance tests
	P3D* a, * b, * c;    //3 points

	int atx, aty, btx, bty, ctx, cty;
	int type, tex, fx, fy; //texturing
};

class Point3D
{
public:
	Point3D(double dX = 0., double dY = 0., double dZ = 0.)
	{
		x = dX;
		y = dY;
		z = dZ;
	}

	double x, y, z;
};

class Camera
{
public:

	double angle1, angle2, angle3; //rotation angles
	double C1, S1, C2, S2, C3, S3; // cos and sin of angles T1,T2 and T3

	double x, y, z, ahead; //observer position
	double zoomFactor;
	int screenCenterX, screenCenterY;
};

class Engine3D
{
public:
	Engine3D(int width, int height, int* pBuffer);
	~Engine3D();

	void clear();
	void set_camera(double ox, double oy, double oz, double ahead, double teta1, double teta2, double teta3, double facr);

	void draw_facet(FACET f);
	void draw_trianglecolor(Point3D& A, int colorA, Point3D& B, int colorB, Point3D& C, int colorC);
	void draw_line(Point3D& p1, Point3D& p2, int color);
	void draw_pixel(Point3D& pPixels, int color);

	void LoadTex();

private:
	Point3D local_ref(const Point3D& pc) const;
	bool compute_projection(Point3D& pPixels, int& ex, int& ey, float& zpx);
	void CalcI12txy(int j);
	void lineH(int x1, int y, int x2);
	void line15(int x1, int y, int x2);
	void line16(int x1, int y);
	void MapTrapez();
	void calctxy(int x);

	Camera _camera;
	int* _pBuffer;
	float* _zbuffer, * _zbuffer16, * _zbuffer256;
	int _Xmax, _Ymax;
	int* tex;
	int gex, gey, gez, dex, dey, dez, gsx, gsy, gsz, dsx, dsy, dsz;
	int a1, a2, b1, b2, a1x1, a2x2;
	int gstx, getx, dstx, detx, gsty, gety, dsty, dety; //texturing
	int ystart, yend;
	int i1tx, i1ty, i2tx, i2ty;
	int i1x, i1y, i1z, i2x, i2y, i2z;
	int atx, btx, aty, bty, itx, ity, az, bz;
	int* pPixels;
	int iz, invz, dinvz; // used by zbuffer
};

#endif
