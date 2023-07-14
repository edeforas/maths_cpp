#include <iostream>
#include <vector>
using namespace std;

inline double sqr(double d)
{ return d*d; }

double min_local_path(double d1,double d2,double d3)
{
    if( (d1<=d2) && (d1<=d3) )
        return d1;

    if(d2<=d3)
        return d2;
    else
        return d3;
}

double DTW(double* s1, int N1, double* s2, int N2)
{
	vector<double> D(N1*N2);

	// compute dist_map square
	for (int i = 0; i < N1; i++)
		for (int j = 0; j < N2; j++)
			D[i*N2 + j] = sqr(s1[i] - s2[j]);

	//disp D
	for (int i = 0; i < N1; i++)
	{
		for (int j = 0; j < N2; j++)
			cout << D[i*N2 + j] << "\t";
		cout << endl;
	}

	// find smalest accumulated path
	for (int i = 1; i < N1; i++) //1st col
		D[i*N2] += D[(i - 1)*N2];
	for (int j = 1; j < N2; j++) // 1st line
		D[j] += D[j - 1];
	for (int i = 1; i < N1; i++) // others
		for (int j = 1; j < N2; j++)
			D[i*N2 + j] += min_local_path(D[(i - 1)*N2 + j - 1], D[i*N2 + j - 1], D[(i - 1)*N2 + j]);

	//disp D
	cout << endl;
	for (int i = 0; i < N1; i++)
	{
		for (int j = 0; j < N2; j++)
			cout << D[i*N2 + j] << "\t";
		cout << endl;
	}

	return D[N1*N2 - 1];
}

int main()
{
	double s1[] = { 0,1,0,0,8,9,6 };
	double s2[] = { 0,0,1,0,9,5 };

	double dist = DTW(s1,7 /*size of s1*/ , s2,6 /*size of s2*/);
	cout << endl << "dist=" << dist << endl;
    return 0;
}
