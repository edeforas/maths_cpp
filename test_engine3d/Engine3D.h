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
	P3D * a, * b, * c;    //3 points

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

	void set_angles(double yaw, double pitch, double roll)
	{
		double dDegToRad = 2. * 3.14159265359 / 360.;

		_yaw = yaw;
		_pitch = pitch;
		_roll = roll;

		_yawCos = cos(_yaw * dDegToRad);
		_yawSin = sin(_yaw * dDegToRad);
		_pitchCos = cos(_pitch * dDegToRad);
		_pitchSin = sin(_pitch * dDegToRad);
		_rollCos = cos(_roll * dDegToRad);
		_rollSin = sin(_roll * dDegToRad);
	}

	void set_origin(double x, double y, double z, double ahead)
	{
		_x = x; _y = y; _z = z;
		_ahead = ahead;
	}

	void set_screen(int width, int height, double zoom)
	{
		screenCenterX = width / 2;
		screenCenterY = height / 2;
		zoomFactor = zoom;
	}

	Point3D local_ref(const Point3D& pc) const
	{
		Point3D pPixels;
		pPixels.x = pc.x;
		pPixels.y = pc.y;
		pPixels.z = pc.z;

		//origin translation
		pPixels.x -= _x;
		pPixels.y -= _y;
		pPixels.z -= _z;

		//yaw rotation
		double tmp = pPixels.x;
		pPixels.x = pPixels.x * _yawCos + pPixels.z * _yawSin;
		pPixels.z = pPixels.z * _yawCos - tmp * _yawSin;

		//pitch rotation
		tmp = pPixels.y;
		pPixels.y = pPixels.z * _pitchSin - pPixels.y * _pitchCos;
		pPixels.z = pPixels.z * _pitchCos + tmp * _pitchSin;

		//roll rotation
		tmp = pPixels.x;
		pPixels.x = pPixels.x * _rollCos + pPixels.y * _rollSin;
		pPixels.y = pPixels.y * _rollCos - tmp * _rollSin;

		// ahead move
		pPixels.z = pPixels.z + _ahead;

		return pPixels;
	}

	bool project(Point3D& pPixels, int& ex, int& ey, float& zpx)
	{
		Point3D pc = local_ref(pPixels);

		//projection sur l'ecran
		ex = int(pc.x * zoomFactor / pc.z + screenCenterX);
		ey = int(pc.y * zoomFactor / pc.z + screenCenterY);

		if ((ex < 0) || (ex >= screenCenterX*2) || (ey < 0) || (ey >= screenCenterY*2))
			return false;

		if (pc.z != 0.)
			zpx = float(1. / pc.z);
		else
			zpx = 0.;

		return true;
	}

private:
	//rotation angles
	double _yawCos, _yawSin, _pitchCos, _pitchSin, _rollCos, _rollSin;
	double _yaw, _pitch, _roll; 

	//observer position
	double _x, _y, _z, _ahead; 

	// screen properties
	double zoomFactor;
	int screenCenterX, screenCenterY;

};

class Engine3D
{
public:
	Engine3D(int width, int height, int* pBuffer);
	~Engine3D();

	void clear();
	void set_camera(double ox, double oy, double oz, double ahead, double yaw, double pitch, double roll, double zoom);

	void draw_facet(FACET f);
	void draw_trianglecolor(Point3D& A, int colorA, Point3D& B, int colorB, Point3D& C, int colorC);
	void draw_line(Point3D& p1, Point3D& p2, int color);
	void draw_pixel(Point3D& pPixels, int color);

	void LoadTex();

private:
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
