#include "ImageIoBmp.h"
#include "Image.h"

#include <fstream>
#include <cassert>

//////////////////////////////////////////////////////////////////////////////
bool ImageIoBmp::read(string sFilename,Image* pImage)
{
    std::fstream fbmp;
    fbmp.open(sFilename.data(),std::ios::binary|std::ios::in);

    //le fichier doit etre ouvert en lecture
    if(! fbmp)
        return false;
    //ouvre le bitmap et verifie son type...

    short sMagic=0;
    fbmp.read( (char *)&sMagic, 2);
    if(sMagic!=0x4D42)
        return 0;

    int iTmp=0;
    short sTmp=0;

    // 1ere structure
    fbmp.read( (char *)&iTmp, 4);
    fbmp.read( (char *)&sTmp, 2);
    fbmp.read( (char *)&sTmp, 2);
    fbmp.read( (char *)&iTmp, 4);

    // 2eme structure
    fbmp.read( (char *)&iTmp, 4);
    int iWidth=0,iHeight=0;
    fbmp.read( (char *)&iWidth, 4);
    fbmp.read( (char *)&iHeight, 4);

    fbmp.read( (char *)&sTmp, 2); // planes

    // lit le nb de bits
    short sBPP=0;
    fbmp.read( (char *)&sBPP, 2); // bpp

    fbmp.read( (char *)&iTmp, 4); //compression
    fbmp.read( (char *)&iTmp, 4); // sizeimage
    fbmp.read( (char *)&iTmp, 4); //ppiX
    fbmp.read( (char *)&iTmp, 4); //ppiY
    fbmp.read( (char *)&iTmp, 4); //clrused
    fbmp.read( (char *)&iTmp, 4); //clr important

    int iNbByte = sBPP >> 3;
    pImage->resize(iWidth, iHeight, iNbByte);

    if(sBPP==24)
    {
        int iLineWidthPerfect=iWidth*iNbByte;
        int iLineWidth=(iLineWidthPerfect+3) %4;
        int iPack=iLineWidth-iLineWidthPerfect;

        // les octets de l'image suivent
        unsigned char cTmp[3]={0}, * pLineY, cTmpR;
        for(int iY=0;iY<iHeight;iY++)
        {
            pLineY=pImage->value(0,iHeight-iY-1,0);
            fbmp.read((char *)pLineY,iLineWidthPerfect);

            //inverse le R et le B
            for(int iC=0;iC<iWidth;iC++)
            {
                cTmpR=pLineY[3*iC+2];
                pLineY[3*iC+2]=pLineY[3*iC];
                pLineY[3*iC]=cTmpR;
            }

            fbmp.read((char *)cTmp,iPack);
        }
    }
    else
    {   //assume grey level, no color LUT for now
        int iLineWidth = iWidth+(iWidth %4);
        int iPack = iLineWidth - iWidth;

        // les octets de l'image suivent
        unsigned char cTmp[3] = { 0 }, * pLineY;

        //read lut, but do not use:
        for (int i = 0; i < 256 * 4; i++)
            fbmp.read((char*)cTmp, 1);

        for (int iY = iHeight-1; iY >=0; iY--)
        {
            pLineY = pImage->value(0, iY, 0);
            fbmp.read((char*)pLineY, iWidth);
            fbmp.read((char*)cTmp, iPack);
        }
    }
    return true;
}
//////////////////////////////////////////////////////////////////////////////
bool ImageIoBmp::write(string sFilename,const Image* pImage)
{
    assert(pImage !=0);

    //cree le fichier et verifie que l'on peut ecrire dedans
    std::fstream fbmp(sFilename.data(),std::ios::binary|std::ios::out);
    if(!fbmp)
        return false;

    int iWidth= pImage->width();
    int iHeight= pImage->height();
    int iPlanes= pImage->planes();

    int iLineWidthPerfect= pImage->width()* pImage->planes();
    int iLineWidth=(iLineWidthPerfect+3);
    iLineWidth-=iLineWidth %4;
    int iPack=iLineWidth-iLineWidthPerfect;

    int iBfhSize=14;
    int iBihSize=40;

    // ecrit bfh
    short sMagic=0x4D42;
    fbmp.write((char*)&sMagic,2);

    int iPaletteSize=0;
    if(iPlanes==1)
        iPaletteSize=256*4;

    int iBufferSize=iLineWidth*iHeight;
    int iSize=iBfhSize+iBihSize+iPaletteSize+iBufferSize;
    fbmp.write((char*)&iSize,4);

    short sZero=0;
    fbmp.write((char*)&sZero,2);
    fbmp.write((char*)&sZero,2);

    int iOffsetB=iBfhSize+iBihSize+iPaletteSize;
    fbmp.write((char*)&iOffsetB,4);

    // ecrit bih
    fbmp.write((char*)&iBihSize,4);
    fbmp.write((char*)&iWidth,4);
    fbmp.write((char*)&iHeight,4);

    short sPlane=1;
    fbmp.write((char*)&sPlane,2);

    short sNbChannels=24;
    if(iPlanes==1)
        sNbChannels=8;
    fbmp.write((char*)&sNbChannels,2);

    int iZero=0;
    fbmp.write((char*)&iZero,4);
    fbmp.write((char*)&iBufferSize,4);

    int iDPI=0xb12;
    fbmp.write((char*)&iDPI,4);
    fbmp.write((char*)&iDPI,4);

    fbmp.write((char*)&iZero,4);
    fbmp.write((char*)&iZero,4);

    if(iPlanes==1)
    {
        for(int i=0;i<256;i++)
        {
            char uc=i;
            fbmp.write(&uc,1);
            fbmp.write(&uc,1);
            fbmp.write(&uc,1);
            uc=0;
            fbmp.write(&uc,1);
        }
    }

    unsigned char cTmp[3] = { 0 };
    const unsigned char *pLineY;
    if(iPlanes==3)
    {
        unsigned char* pLineOut=new unsigned char[iLineWidthPerfect+3];
        for(int iY=0;iY<iHeight;iY++)
        {
            pLineY= pImage->value(0,iHeight-iY-1,0);
            for( int iC=0;iC<iLineWidthPerfect;iC+=3)
            {
                pLineOut[iC]=pLineY[iC+2];
                pLineOut[iC+1]=pLineY[iC+1];
                pLineOut[iC+2]=pLineY[iC];
            }

            fbmp.write((const char*)pLineOut,iLineWidthPerfect);
            fbmp.write((const char*)cTmp,iPack);
        }
        delete [] pLineOut;
    }

    if(iPlanes==1)
    {
        for(int iY=0;iY<iHeight;iY++)
        {
            pLineY= pImage->value(0,iHeight-iY-1,0);
            fbmp.write((const char*)pLineY,iLineWidthPerfect);
            fbmp.write((const char*)cTmp,iPack);
        }
    }

    fbmp.close();
    return true;
}
//////////////////////////////////////////////////////////////////////////////
