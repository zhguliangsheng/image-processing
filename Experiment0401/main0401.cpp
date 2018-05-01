#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "bmpFile.h"
#include "RmwEdgeDetect.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     从彩色图像中选择一个位面
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwExtractOneChannelFromRGB(BYTE *pRGBImg,
								 int width,int height,
								 int R2B0G1,BYTE *pResImg)
{   
	BYTE *pRgbCur,*pResCur;
    int i,ImgSize=width*height;


    pRgbCur=pRGBImg+R2B0G1;
	pResCur=pResImg;
	for(i=0;i<ImgSize;i++,pRgbCur+=3) *(pResCur++)=*pRgbCur;
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     测试
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void test1()
{
	clock_t t1,t2;
	int width,height;

	BYTE *pRGBImg=RmwRead24BitBmpFile2Img("Dune.bmp",&width,&height);
	BYTE *pGryImg=new BYTE [width*height];
	BYTE *pResImg=new BYTE [width*height];
	BYTE *pTmpImg=new BYTE [width*height];
	
	RmwExtractOneChannelFromRGB(pRGBImg,width,height,0,pGryImg);
	RmwWrite8BitImg2BmpFile(pGryImg,width,height,"Dune_Blue.bmp");

	RmwGradientGrayImage(pGryImg,width,height,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"Dune_Gradient.bmp");

	RmwRobertGrayImage(pGryImg,width,height,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"Dune_Robert.bmp");

	RmwSobelGrayImage(pGryImg,width,height,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"Dune_Sobel.bmp");

	RmwGradientByPrewitt(pGryImg,width,height,1,1,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"Dune_Prewitt.bmp");

	RmwGradientByPrewitt(pGryImg,width,height,2,2,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"Dune_Prewitt5x5.bmp");


	RmwShenJunEdge(pGryImg,pTmpImg,width,height,0.3,pResImg); 
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"Dune_Shenjun3.bmp");

	RmwShenJunEdge(pGryImg,pTmpImg,width,height,0.7,pResImg); 
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"Dune_Shenjun7.bmp");

	delete pRGBImg;
	delete pGryImg;
	delete pResImg;
	delete pTmpImg;
	return;
}

void test2()
{
	clock_t t1,t2;
	int width,height;

	BYTE *pGryImg=RmwRead8BitBmpFile2Img("rice.bmp",&width,&height);
	BYTE *pResImg=new BYTE [width*height];
	BYTE *pTmpImg=new BYTE [width*height];
	
	RmwGradientGrayImage(pGryImg,width,height,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"rice_Gradient.bmp");

	RmwRobertGrayImage(pGryImg,width,height,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"rice_Robert.bmp");

	RmwSobelGrayImage(pGryImg,width,height,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"rice_Sobel.bmp");

	RmwGradientByPrewitt(pGryImg,width,height,1,1,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"rice_Prewitt.bmp");

	RmwGradientByPrewitt(pGryImg,width,height,2,2,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"rice_Prewitt5x5.bmp");


	RmwShenJunEdge(pGryImg,pTmpImg,width,height,0.3,pResImg); 
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"rice_Shenjun3.bmp");

	RmwShenJunEdge(pGryImg,pTmpImg,width,height,0.7,pResImg); 
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"rice_Shenjun7.bmp");

	delete pGryImg;
	delete pResImg;
	delete pTmpImg;
	return;
}
 
void test3()
{
	clock_t t1,t2;
	int width,height;

	//BYTE *pGryImg=RmwRead8BitBmpFile2Img("rice.bmp",&width,&height);
	BYTE *pGryImg=RmwRead8BitBmpFile2Img("FJ.bmp",&width,&height);
	BYTE *pResImg=new BYTE [width*height];
	BYTE *pTmpImg=new BYTE [width*height];

	RmwGradientGrayImage(pGryImg,width,height,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"FJ_Gradient.bmp");

	RmwRobertGrayImage(pGryImg,width,height,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"FJ_Robert.bmp");

	RmwSobelGrayImage(pGryImg,width,height,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"FJ_Sobel.bmp");

	RmwGradientByPrewitt(pGryImg,width,height,1,1,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"FJ_Prewitt.bmp");

	RmwGradientByPrewitt(pGryImg,width,height,2,2,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"FJ_Prewitt5x5.bmp");


	RmwShenJunEdge(pGryImg,pTmpImg,width,height,0.3,pResImg); 
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"FJ_Shenjun3.bmp");

	RmwShenJunEdge(pGryImg,pTmpImg,width,height,0.7,pResImg); 
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"FJ_Shenjun7.bmp");

	RmwShenJunEdge(pGryImg,pTmpImg,width,height,0.5,pResImg); 
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"FJ_Shenjun5.bmp");

	RmwShenJunEdge(pGryImg,pTmpImg,width,height,0.9,pResImg); 
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"FJ_Shenjun9.bmp");

	delete pGryImg;
	delete pResImg;
	delete pTmpImg;
	return;
}


void test4()
{
	clock_t t1,t2;
	int width,height;

	BYTE *pGryImg=RmwRead8BitBmpFile2Img("frame0110.bmp",&width,&height);
	BYTE *pResImg=new BYTE [width*height];
	BYTE *pTmpImg=new BYTE [width*height];

	RmwGradientGrayImage(pGryImg,width,height,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"frame0110_Gradient.bmp");

	RmwRobertGrayImage(pGryImg,width,height,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"frame0110_Robert.bmp");

	RmwSobelGrayImage(pGryImg,width,height,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"frame0110_Sobel.bmp");

	RmwGradientByPrewitt(pGryImg,width,height,1,1,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"frame0110_Prewitt.bmp");

	RmwGradientByPrewitt(pGryImg,width,height,2,2,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"frame0110_Prewitt5x5.bmp");


	RmwShenJunEdge(pGryImg,pTmpImg,width,height,0.1,pResImg); 
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"frame0110_Shenjun3.bmp");

	RmwShenJunEdge(pGryImg,pTmpImg,width,height,0.9,pResImg); 
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"frame0110_Shenjun7.bmp");

	delete pGryImg;
	delete pResImg;
	delete pTmpImg;
	return;
}


void main()
{
	test1();
	//test2();
	//test3();
	//test4();
	return;
}

