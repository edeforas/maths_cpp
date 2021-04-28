#include <iostream>
#include <vector>
using namespace std;

#include <vector>
#include <cmath>

#define PI (3.1415926535897932384626433832795)

class OneDir //todo optimize
{
public:
    OneDir(double dirx,double diry)
    {
        xd=dirx; yd=diry;
        bContainsPoints=false;
    }

    void add_point(double x,double y)
    {
        if(bContainsPoints==false)
        {
            xa=x; ya=y; xb=x; yb=y;
            aDotB=0.;
            bContainsPoints=true;
            return;
        }

        double aDotC=(x-xa)*xd+(y-ya)*yd;
        if(aDotC<0.)
        {
            //replace A
            xa=x; ya=y; aDotB=(xa-xb)*xd+(ya-yb)*yd;


        }
        else if(aDotC>aDotB)
        {
            //replace B
            xb=x; yb=y; aDotB=(xa-xb)*xd+(ya-yb)*yd;

        }
    }

    double xa,ya,xb,yb;
    double aDotB;
    double xd,yd;
    bool bContainsPoints;
};


class BoundingPolyFixedCalliper
{
public:
    BoundingPolyFixedCalliper(int iNbVertex)
    {
        _iNbVertex=iNbVertex;
        _iNbDir=iNbVertex/2;

        for(int i=0;i<_iNbDir;i++)
        {
            double dAngle=(double)i/(double)_iNbDir* PI;
            _allDir.push_back(OneDir(sin(dAngle),cos(dAngle)));
        }
    }

    void add_point(double x,double y)
    {
        for(int i=0;i<_iNbDir;i++)
        {
            _allDir[i].add_point(x,y);
        }
    }

    void get_bounding_points(vector<double>& x,vector<double>& y)
    {
        x.clear();
        y.clear();
        for(unsigned int i=0;i<_allDir.size();i++)
        {
            x.push_back(_allDir[i].xa);
            y.push_back(_allDir[i].ya);

            x.push_back(_allDir[i].xb);
            y.push_back(_allDir[i].yb);
        }
    }

private:
    int _iNbVertex;
    int _iNbDir;

    vector<OneDir> _allDir;
};

int main()
{
    BoundingPolyFixedCalliper rp(8);

    rp.add_point(1,1);
    rp.add_point(4,10);
    rp.add_point(-1,-6);
    rp.add_point(-4,4);

    vector<double> x,y;
    rp.get_bounding_points(x,y);

    for(unsigned int i=0;i<x.size();i++)
    {
        cout << "point " << i << " : " << x[i] << " " << y[i] << endl;
    }

    return 0;
}
