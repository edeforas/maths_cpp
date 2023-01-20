#include <iostream>
#include <string>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////
class RealDouble
{
public:
	RealDouble(double r)
	{
		_r = r;
	}

	RealDouble& operator+=(const RealDouble& b)
	{
		_r += b._r;
		return *this;
	}

	RealDouble operator+(const RealDouble& b)
	{
		return RealDouble(b._r + _r);
	}

	RealDouble operator*(const RealDouble& b)
	{
		return RealDouble(b._r * _r);
	}

	RealDouble operator/(const RealDouble& b)
	{
		return RealDouble(_r / b._r);
	}

	friend std::ostream& operator<<(std::ostream& os, const RealDouble& r)
	{
		os << r._r;
		return os;
	}

private:
	double _r;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
class RealFloat
{
public:
	RealFloat(float r)
	{
		_r = r;
	}

	RealFloat& operator+=(const RealFloat& b)
	{
		_r += b._r;
		return *this;
	}

	RealFloat operator+(const RealFloat& b)
	{
		return RealFloat(b._r + _r);
	}

	RealFloat operator*(const RealFloat& b)
	{
		return RealFloat(_r * b._r);
	}

	RealFloat operator/(const RealFloat& b)
	{
		return RealFloat(_r / b._r);
	}

	friend std::ostream& operator<<(std::ostream& os, const RealFloat& r)
	{
		os << r._r;
		return os;
	}

private:
	float _r;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
class Float8q8
{
public:
	Float8q8(float f)
	{
		_r = to_F8Q8(f);
	}

	Float8q8(short r)
	{
		_r = r;
	}

	Float8q8& operator+=(const Float8q8& b)
	{
		_r += b._r;
		return *this;
	}

	Float8q8 operator+(const Float8q8& b)
	{
		return Float8q8(to_float(b._r) + to_float(_r));
	}

	Float8q8 operator*(const Float8q8& b)
	{
		return Float8q8(to_float(_r) * to_float(b._r));
	}

	Float8q8 operator/(const Float8q8& b)
	{
		return Float8q8(to_float(_r) / to_float(b._r));
	}

	friend std::ostream& operator<<(std::ostream& os, const Float8q8& r)
	{
		os << to_float(r._r);
		return os;
	}

private:
	static float to_float(short c)
	{
		int iPow= (c>>8) ;
		int iMant= c&0xff;
		return (float)(iMant * pow(2.f, iPow));
	}

	static short to_F8Q8(float f)
	{
		int iPow;
		float fmant = frexp(f, &iPow);
		int iMant = (int)(fmant *128);
		
		short _r = ((iPow - 7) << 8) + (char)iMant;
		return _r;
	}

	short _r;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
class Fixpt8q8
{
public:
	Fixpt8q8(float f)
	{
		_r = to_F8Q8(f);
	}

	Fixpt8q8(short r)
	{
		_r = r;
	}

	Fixpt8q8& operator+=(const Fixpt8q8& b)
	{
		_r += b._r;
		return *this;
	}

	Fixpt8q8 operator+(const Fixpt8q8& b)
	{
		return Fixpt8q8(to_float(b._r) + to_float(_r));
	}

	Fixpt8q8 operator*(const Fixpt8q8& b)
	{
		return Fixpt8q8(to_float(_r) * to_float(b._r));
	}

	Fixpt8q8 operator/(const Fixpt8q8& b)
	{
		return Fixpt8q8(to_float(_r) / to_float(b._r));
	}

	friend std::ostream& operator<<(std::ostream& os, const Fixpt8q8& r)
	{
		os << to_float(r._r);
		return os;
	}

private:
	static float to_float(short c)
	{
		return ((float)c)/256.f;
	}

	static short to_F8Q8(float f)
	{
		return (short)(f * 256.f + 0.5f);
	}

	short _r;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
class FixptTanh8
{
public:
	FixptTanh8(float f)
	{
		_r = to_F8Q8(f);
	}

	FixptTanh8(char r)
	{
		_r = r;
	}

	FixptTanh8& operator+=(const FixptTanh8& b)
	{
		_r += b._r;
		return *this;
	}

	FixptTanh8 operator+(const FixptTanh8& b)
	{
		return FixptTanh8(to_float(b._r) + to_float(_r));
	}

	FixptTanh8 operator*(const FixptTanh8& b)
	{
		return FixptTanh8(to_float(_r) * to_float(b._r));
	}

	FixptTanh8 operator/(const FixptTanh8& b)
	{
		return FixptTanh8(to_float(_r) / to_float(b._r));
	}

	friend std::ostream& operator<<(std::ostream& os, const FixptTanh8& r)
	{
		os << to_float(r._r);
		return os;
	}

private:
	static float to_float(char c)
	{
		return atanhf(((float)c)/127.f)*10.f;
	}

	static char to_F8Q8(float f)
	{
		if (f > 25)
			f = 25;

		float ft = tanhf(f*0.1f);
		return (char)(ft*127.f+0.5f);
	}

	char _r;
};

// choose your real number implementation below:
//typedef float real;
//typedef double real;
//typedef long double real;
//typedef RealDouble real; // same as double but in a class
//typedef RealFloat real; // same as float but in a class
//typedef Float8q8 real;
//typedef Fixpt8q8 real;
typedef FixptTanh8 real;
int main()
{
	cout << "sizeof(real)=" << sizeof(real) << endl;

	// newton step to sqrt(2) to test accuracy
	real x = 2.f;
	real u = 1.f;
	for (int i = 0; i < 10; i++)
	{
		u = real(0.5f)*(u + real(x) / u);
		cout << "i=" << i << " x=" << x << " u("<< i <<")=" << u << " u(" << i<< ")**2=" << u * u << endl;
	}

	return 0;
}