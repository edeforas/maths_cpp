// some experiments with the Pascal triangle :
// conjectures ideas from https://images.math.cnrs.fr/Une-conjecture-sur-le-triangle-de-Pascal.html

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

void first_line(vector<int>& line)
{
	line[0] = 1;
	for (int i = 1; i < line.size(); i++)
		line[i] = 0;
}

void next_line(vector<int>& line)
{
	int a = 1, b;
	for (int i = 1; i < line.size(); i++)
	{
		b = line[i];
		line[i] = a + b;
		a = b;
	}
}

void draw(int iMax)
{
	cout << "1/ print a small pascal triangle:" << endl;

	// create first line
	vector<int> line(iMax);
	first_line(line);

	for(int l =1;l<iMax;l++)
	{
		// print the line before the first diagonal
		for (int i = 0; i < l; i++)
			cout << setw(5) << line[i];
		cout << endl;

		next_line(line);
	}			
}

void draw_shierpinsky(int iMax)
{
	cout << endl << "2/ print a pascal triangle using number parity, show the Shierpinsky fractal:" << endl;

	// create first line
	vector<int> line(iMax);
	first_line(line);

	for (int l = 1; l < iMax; l++)
	{
		// print the line before the first diagonal
		for (int i = 0; i < l; i++)
			if ( (line[i] % 2) == 0)
				cout << " ";
			else
				cout << "*";

		cout << endl;

		next_line(line);
	}
	cout << endl;
}


void main()
{
	draw(17);
	draw_shierpinsky(64);
}