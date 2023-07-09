// Programme de Compression des images par Ondelettes
// Par Rania ELDAHDAH, Etienne DEFORAS et Morgan ROCHE
// version maj du 10/06/2023 : EDF

#ifndef _wavelets_
#define _wavelets_

class Wavelets
{
public:
	Wavelets();

	void transform_wavelet_1d(int nn, double img[256]);
	void transform_wavelet_2d(double img[256][256], double ond1[256][256]);

	void untransform_wavelet_1d(int n, double img[256]);
	void untransform_wavelet_2d(double ond2[256][256], double fic[256][256]);

	void compress(double ond1[256][256], double ond2[256][256]);
	void copy(const double source[256][256], double cible[256][256]);
	double compare(double tab1[256][256], double tab2[256][256]);

private:
	void initcoef(int n);
	double compute_error(double ond2[256][256], int xmin, int xmax, int ymin, int ymax, double pourcent);
	void zero(double cible[256][256], int xmin, int xmax, int ymin, int ymax, double erreur);

	int _ncof;
	double* _paire;
	double* _impaire;
};

#endif
