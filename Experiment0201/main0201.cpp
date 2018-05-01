#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "bmpFile.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwGryImageGamaCorrect_Slow(BYTE *pGryImg,double gama,int nSize)
{  
	for(int i=0;i<nSize;i++) 
	{
		*(pGryImg+i) = min(255,pow(*(pGryImg+i),gama));
	}
	return;
}

void RmwGryImageGamaCorrect_Fast(BYTE *pGryImg,double gama,int nSize)
{  
	int gLUT[256];
	int i;
	for(i=1;i<256;i++) gLUT[i]=min(255,pow(i,gama));
	for(i=0;i<nSize;i++) 
	{
		*(pGryImg+i) = gLUT[*(pGryImg+i)];
	}
	return;
}

void Example21()
{
	clock_t t1,t2;
	int width,height;
	int nTstTimes=10;

	BYTE *pGryImgC=RmwRead8BitBmpFile2Img("GryImgC.bmp",&width,&height);
 
	t1=clock();
	for(int i=0;i<nTstTimes;i++) 
	{
		//RmwGryImageGamaCorrect_Slow(pGryImgC,1.6,width*height);
		RmwGryImageGamaCorrect_Fast(pGryImgC,1.6,width*height);
	}
	t2=clock();
	printf("Time Consuming = %.3lf ms\n",1.0*(t2-t1)/nTstTimes);

	RmwWrite8BitImg2BmpFile(pGryImgC,width,height,"GryImgC_gama.bmp");

	delete pGryImgC;
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//²ÊÉ«Í¼×ª»Ò¶ÈÍ¼
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwRGBImg2GryImg0(BYTE *pRGBImg,int width,int height,BYTE *pGryImg)
{
    BYTE *pRGB,*pGry,*pEnd=pRGBImg+3*width*height;
	double gry;
    for(pRGB=pRGBImg,pGry=pGryImg;pRGB<pEnd;)
	{
		gry  = *(pRGB++)*0.114; //B
		gry += *(pRGB++)*0.587; //G
		gry += *(pRGB++)*0.299; //R
		*(pGry++) = (int)(gry);
	}
    return;
}

void RmwRGBImg2GryImg1(BYTE *pRGBImg,int width,int height,BYTE *pGryImg)
{
    BYTE *pRGB,*pGry,*pEnd=pRGBImg+3*width*height;
	double LUTR[256],LUTG[256],LUTB[256],gry;
	int i;
	
	for(i=0;i<256;i++)
	{
		LUTR[i]=0.299*i;
        LUTG[i]=0.587*i;
		LUTB[i]=0.114*i;
	}
    for(pRGB=pRGBImg,pGry=pGryImg;pRGB<pEnd;)
	{
		gry =  LUTB[*(pRGB++)];
		gry += LUTG[*(pRGB++)];
		gry += LUTR[*(pRGB++)];
		*(pGry++) = (int)(gry);
	}
    return;
}

void RmwRGBImg2GryImg2(BYTE *pRGBImg,int width,int height,BYTE *pGryImg)
{
    BYTE *pRGB,*pGry,*pEnd=pRGBImg+3*width*height;
	int LUTR[256],LUTG[256],LUTB[256];
	int i,gry;
	
	for(i=0;i<256;i++)
	{
		LUTR[i]=(int)(0.299*i*1000);
        LUTG[i]=(int)(0.587*i*1000);
		LUTB[i]=(int)(0.114*i*1000);
	}
    for(pRGB=pRGBImg,pGry=pGryImg;pRGB<pEnd;)
	{
		gry =  LUTB[*(pRGB++)];
		gry += LUTG[*(pRGB++)];
		gry += LUTR[*(pRGB++)];
		*(pGry++) = gry/1000;
	}
    return;
}

void RmwRGBImg2GryImg3(BYTE *pRGBImg,int width,int height,BYTE *pGryImg)
{
    BYTE *pRGB,*pGry,*pEnd=pRGBImg+3*width*height;
	int LUTR[256],LUTG[256],LUTB[256];
	int i,gry;
	
	for(i=0;i<256;i++)
	{
		LUTR[i]=(int)(0.299*i*1024);
        LUTG[i]=(int)(0.587*i*1024);
		LUTB[i]=(int)(0.114*i*1024);
	}
    for(pRGB=pRGBImg,pGry=pGryImg;pRGB<pEnd;)
	{
		gry  = LUTB[*(pRGB++)];
		gry += LUTG[*(pRGB++)];
		gry += LUTR[*(pRGB++)];
		*(pGry++) = gry>>10;
	}
    return;
}

void Example22()
{
	clock_t t1,t2;
	int width,height;
	int nTstTimes=1000;

	BYTE *pRGBImg = RmwRead24BitBmpFile2Img("RgbImg.bmp",&width,&height);
    BYTE *pGryImg = new BYTE [width*height];

	t1=clock();
	for(int i=0;i<nTstTimes;i++) 
	{
		//RmwRGBImg2GryImg0(pRGBImg,width,height,pGryImg); //26.187ms
		//RmwRGBImg2GryImg1(pRGBImg,width,height,pGryImg); //22.011ms
		//RmwRGBImg2GryImg2(pRGBImg,width,height,pGryImg); //3.835ms
		RmwRGBImg2GryImg3(pRGBImg,width,height,pGryImg); //2.443ms
	}
	t2=clock();
	printf("Time Consuming = %.3lf ms\n",1.0*(t2-t1)/nTstTimes);

	RmwWrite8BitImg2BmpFile(pGryImg,width,height,"ResGryImg.bmp");

	delete pRGBImg;
	delete pGryImg;
	return;
}
void main()
{
	//Example21();
	Example22();
}
