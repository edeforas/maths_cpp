#ifndef _ImageIoBmp_
#define _ImageIoBmp_

#include "Image.h"

#include <string>
using namespace std;

class ImageIoBmp
{
public:
    bool write(string sFilename,const Image* pImage);
    bool read(string sFilename, Image* pImage);
};

#endif