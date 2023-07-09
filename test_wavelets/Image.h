#ifndef _Image_
#define _Image_

#include <assert.h>


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
        _data=new unsigned char[_iW*_iH*_iP];
    }

    virtual ~Image()
    {
        delete [] _data;
    }

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

    void resize(int iW,int iH,int iP)
    {
        assert(iW>=0);
        assert(iH>=0);
        assert(iP>=0);

        delete [] _data;
        _iW=iW;
        _iH=iH;
        _iP=iP;
        _data=new unsigned char[_iW*_iH*_iP];
    }

    const unsigned char* value(int iX,int iY,int iP=0) const
    {
        assert(iX>=0);
        assert(iY>=0);
        assert(iP>=0);
        assert(iX<_iW);
        assert(iY<_iH);
        assert(iP<_iP);

        return _data +iP +_iP*_iW*iY +iX*_iP;
    }

    unsigned char* value(int iX, int iY, int iP = 0)
    {
        assert(iX >= 0);
        assert(iY >= 0);
        assert(iP >= 0);
        assert(iX < _iW);
        assert(iY < _iH);
        assert(iP < _iP);

        return _data + iP + _iP * _iW * iY + iX * _iP;
    }


    unsigned char& operator()(int iX,int iY,int iP=0)
    {
        assert(iX>=0);
        assert(iY>=0);
        assert(iP>=0);
        assert(iX<_iW);
        assert(iY<_iH);
        assert(iP<_iP);

        return *(_data +iP +_iP*_iW*iY +iX*_iP);
    }

private:
    int _iW,_iH,_iP;
    unsigned char * _data;
};


#endif
