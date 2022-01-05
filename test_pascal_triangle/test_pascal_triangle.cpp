// some experiments with the Pascal triangle :
// conjectures ideas from https://images.math.cnrs.fr/Une-conjecture-sur-le-triangle-de-Pascal.html

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

void compute_first_line(vector<int>& line)
{	// init to [ 1 0 0 ...]
	line[0] = 1;
	for (int i = 1; i < line.size(); i++)
		line[i] = 0;
}

void compute_line(vector<int>& line,int iLine)
{   // if we assume the first col is 1: we can compute all the line
	line[0] = 1;
	for (int p= 1; p < line.size(); p++)
	{
		line[p] = line[p - 1] * (iLine + 1 - p) / (p );
	}
}

void compute_next_line(vector<int>& line)
{   // if we assume the first col is 1: we can compute next line
	int a = 1, b;
	for (int i = 1; i < line.size(); i++)
	{
		b = line[i];
		line[i] = a + b;
		a = b;
	}
}

void compute_prev_line(vector<int>& line)
{   // if we assume the first col is 1: we can compute prev line
	int a = 1;
	for (int i = 1; i < line.size(); i++)
	{
		a = line[i]-a;
		line[i]=a;
	}
}

void print_line(vector<int>& line)
{
	// print the line before the first diagonal
	for (int i = 0; i < line.size(); i++)
		cout << setw(5) << line[i];
	cout << endl;
}

void draw_triangle(int iMax)
{
	cout << "Print a small pascal triangle:" << endl;

	// create first line
	vector<int> line(iMax);
	compute_first_line(line);

	for(int l =1;l<iMax;l++)
	{
		print_line(line);
		compute_next_line(line);
	}			
}

void draw_triangle_compute_each_line(int iMax)
{
	cout << "Print a small pascal triangle, compute each line:" << endl;

	vector<int> line(iMax);

	for (int l = 0; l < iMax; l++)
	{
		print_line(line);
		compute_line(line,l);
	}
}

void draw_triangle_negative(int iMax)
{
	cout << "Print the negative part using the reccurence equation in reverse (1st solution):" << endl;

	// create first line
	vector<int> line(iMax);
	compute_first_line(line);

	// go to the minimum (negative part)
	for (int l = 0; l >= -iMax; l--)
		compute_prev_line(line);

	for (int l = -iMax; l < iMax; l++)
	{
		print_line(line);
		compute_next_line(line);
	}
}

void draw_shierpinsky(int iMax)
{
	cout << endl << "Print a pascal triangle using number parity, show the Shierpinsky fractal:" << endl;

	// create first line
	vector<int> line(iMax);
	compute_first_line(line);

	for (int l = 1; l < iMax; l++)
	{
		// print the line before the first diagonal, parity only
		for (int i = 0; i < l; i++)
			if ( (line[i] % 2) == 0)
				cout << " ";
			else
				cout << "*";

		cout << endl;

		compute_next_line(line);
	}
	cout << endl;
}


void main()
{
	draw_triangle(17);
//	draw_triangle_negative(13);
	draw_triangle_compute_each_line(17);
//	draw_shierpinsky(64);
}