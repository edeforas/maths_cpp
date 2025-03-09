// Programme de Compression des images par Ondelettes
// by Rania ELDAHDAH, Etienne DEFORAS and Morgan ROCHE
// updated 3/9/2025

#ifndef _Wavelets_
#define _Wavelets_

#include <vector>
using namespace std;

class Wavelets
{
public:
	Wavelets(int nCoef=4);

	void transform(const vector<double>& a, vector<double>& b);
	void untransform(const vector<double>& a, vector<double>& b);

private:
	void init(int nCoef);

	int _nCoef;
	const double* _even;
	vector<double> _odd;
};

#endif
