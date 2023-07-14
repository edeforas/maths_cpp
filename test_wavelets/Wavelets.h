// Programme de Compression des images par Ondelettes
// Par Rania ELDAHDAH, Etienne DEFORAS et Morgan ROCHE
// version maj du 10/06/2023 : EDF

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
	void initcoef(int nCoef);

	int _nCoef;
	double* _paire;
	double* _impaire;
};

#endif
