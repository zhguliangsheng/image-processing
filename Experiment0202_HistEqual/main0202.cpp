#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "bmpFile.h"

void F1(BYTE *pImg,int width,int height)
{
	BYTE *pCur,*pEnd=pImg+width*height;
	int histogram[256];
	int A,g;
	int LUT[256];

	memset(histogram,0,sizeof(int)*256);
	for(pCur=pImg;pCur<pEnd;) histogram[*(pCur++)]++;
	for(LUT[0]=0,A=histogram[0],g=1;g<256;g++)
	{
		A=A+histogram[g];
		LUT[g]=255*A/(width*height);
	}
	for(pCur=pImg;pCur<pEnd;)*(pCur++)=LUT[*pCur];
	return;
}
/*
void F2(BYTE *pImg,int width,int height)
{
	BYTE *pCur,*pEnd=pImg+width*height;
	int histogram[256];

	memset(histogram,0,sizeof(int)*256);
	for(pCur=pImg;pCur<pEnd;) histogram[*(pCur++)]++;
	N=0;
	for(i=0;i<256;i++) 
	{
		if (histogram[i]) N++;
	}
	int A[256];
	for(g=0;g<256;g++)
	{
		A[g]=0;
		for(i=0;i<g;i++)
		{
			A[g]+=histogram[i];
		}
	}
	for(g=0;g<256;g++) LUT[g]=(N*A[g]/(width*height))*(255/N);

	//for(g=0;g<256;g++) LUT[g]=(N*A[g])/A[255]; ///A[255];//(width*height);


	for(pCur=pImg;pCur<pEnd;)*(pCur++)=LUT[*pCur];
	return;
}

void F3(BYTE *pImg,int width,int height)
{
	BYTE *pCur,*pEnd=pImg+width*height;
	int g,histogram[256],LUT[256],sum=0;

	memset(histogram,0,sizeof(int)*256);
	for(pCur=pImg;pCur<pEnd;) histogram[*(pCur++)]++;
	for(g=0;g<256;g++)
	{
		sum +=histogram[g];
		LUT[g]=255*sum/(width*height);
	}
	for(pCur=pImg;pCur<pEnd;)*(pCur++)=LUT[*pCur];
	return;
}
*/

void RmwHistogramEqualize(BYTE *pImg,int width,int height)
{
	BYTE *pCur,*pEnd=pImg+width*height;
	unsigned int hist[256];
	int LUT[256],i,sum;
    
	memset(hist,0,sizeof(int)*256);
	for(pCur=pImg;pCur<pEnd;) hist[*(pCur++)]++;
	for(sum=hist[0],LUT[0]=0,i=1;i<256;i++) 
	{
		sum = sum+hist[i];
		LUT[i]=255*sum/(width*height);
	}
	for(pCur=pImg;pCur<pEnd;) *(pCur++)=LUT[*pCur];
	return;
}


void main()
{
	int width,height;
	clock_t t1,t2;

	BYTE *pGryImg=RmwRead8BitBmpFile2Img("testHistEqual.bmp",&width,&height);
	//BYTE *pGryImg=RmwRead8BitBmpFile2Img("GryImgC.bmp",&width,&height);
	printf("%d,%d",width,height);
	t1=clock();
    //for(int i=0;i<1000;i++);
		//RmwHistogramEqualize(pGryImg,width,height);
		F1(pGryImg,width,height);
	t2=clock();
	RmwWrite8BitImg2BmpFile(pGryImg,width,height,"testHistEqual_Res.bmp");

	printf("\n%lf ms\n",(t2-t1)/1000.0);
	delete pGryImg;
	return;
}
