#ifndef _Image_
#define _Image_

#include <cassert>
#include <vector>
using namespace std;

class Image
{
public:
    Image(int iW=0,int iH=0,int iP=1)
    {
        assert(iW>=0);
        assert(iH>=0);
        assert(iP>=0);

        _iW=iW;
        _iH=iH;
        _iP=iP;
        _data.resize(_iW*_iH*_iP);
    }

    virtual ~Image()
    { }

    int width() const
    {
        return _iW;
    }

    int height() const
    {
        return _iH;
    }

    int planes() const
    {
        return _iP;
    }

    int size() const
    {
        return _iH * _iW * _iP;
    }

    void resize(int iW,int iH,int iP)
    {
        assert(iW>=0);
        assert(iH>=0);
        assert(iP>=0);

        _iW=iW;
        _iH=iH;
        _iP=iP;
        _data.resize(_iW * _iH * _iP);
    }

    const unsigned char* value(int iX,int iY,int iP=0) const
    {
        assert(iX>=0);
        assert(iY>=0);
        assert(iP>=0);
        assert(iX<_iW);
        assert(iY<_iH);
        assert(iP<_iP);

        return _data.data() + iP + _iP * _iW * iY + iX * _iP;
    }

    unsigned char* value(int iX, int iY, int iP = 0)
    {
        assert(iX >= 0);
        assert(iY >= 0);
        assert(iP >= 0);
        assert(iX < _iW);
        assert(iY < _iH);
        assert(iP < _iP);

        return _data.data() + iP + _iP * _iW * iY + iX * _iP;
    }


    unsigned char& operator()(int iX,int iY,int iP=0)
    {
        assert(iX>=0);
        assert(iY>=0);
        assert(iP>=0);
        assert(iX<_iW);
        assert(iY<_iH);
        assert(iP<_iP);

        return *(_data.data() + iP + _iP * _iW * iY + iX * _iP);
    }

    void to_double(vector<double>& img)
    {
        img.resize(size());
        for (int ligne = 0; ligne < width(); ligne++)
            for (int colonne = 0; colonne < height(); colonne++)
                img[colonne + ligne * width()] = (*this)(ligne, colonne);
    }

private:
    int _iW,_iH,_iP;
    vector<unsigned char> _data;
};

#endif
