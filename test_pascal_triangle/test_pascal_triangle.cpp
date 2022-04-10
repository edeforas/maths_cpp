// some experiments with the Pascal triangle :
// conjectures ideas from https://images.math.cnrs.fr/Une-conjecture-sur-le-triangle-de-Pascal.html

#include <iostream>
#include <iomanip>
#include <cassert>
using namespace std;

class PascalCell
{
public:
	PascalCell()
	{
		init_at_line(1);
	}

	void init_at_line(int iLine)
	{
		_p = 0;
		_n = iLine;
		_iValue = 1;
	}

	void right() 
	{
		_iValue = (_iValue * (_n - _p)) / (_p + 1);
		_p++;
	}

	void left() {}
	void up()
	{
		_iValue = (_iValue * (_n - _p)) / (_n );
		_n--;	
	}
	void down()
	{
		_iValue = (_iValue * (_n + 1 - _p)) / (_n + 1);
		_n++;
	}

	int value()
	{
		return _iValue;
	}

private:
	int _iValue;
	int _n;
	int _p;
};

void draw_triangle(int iMax)
{
	cout << "Print a Pascal triangle:" << endl;

	for(int l =0;l<iMax;l++)
	{
		PascalCell pc;
		pc.init_at_line(l);

		for (int i = 0; i <= l; i++)
		{
			cout << setw(5) << pc.value();
			pc.right();
		}
		cout << endl;
	}
}

int count(int iVal)
{
	// search and count for occurences
	int iNbVal = 0;
	PascalCell pc;
	pc.init_at_line(iVal);
	pc.right();
	assert(pc.value() == iVal);

	int v = 0;
	do
	{
		v = pc.value();
		cout << v << " ";

		if (v == iVal)
		{
			iNbVal++;
			pc.right();
		}
		
		if (v < iVal)
		{
			pc.down();
			if (pc.value() > iVal)
			{
				pc.up();
				pc.right();
			}
		}

		if (v > iVal)
		{
			pc.up();
			if (pc.value() < iVal)
			{
				pc.down();
				pc.right();
			}
		}
	} while (v > 1);

	return iNbVal;
}

void main()
{
	draw_triangle(13);

	for (int i = 2; i < 13; i++)
		cout << "val=" << i << " count=" << count(i) << endl;
}