#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "bmpFile.h"
 
void RmwGryImageADivB_Slow(BYTE *pGryImgA,BYTE *pGryImgB,int nSize)
{  
	int B;
	for(int i=0;i<nSize;i++) //*(pGryImgA+i) = *(pGryImgA+i)/(*(pGryImgB+i)+0.00001);
	{
		B=*(pGryImgB+i);
		if (B) *(pGryImgA+i) = *(pGryImgA+i)/B;
		else *(pGryImgA+i)=255;
	}
	return;
}

void RmwGryImageADivB_Fast(BYTE *pGryImgA,BYTE *pGryImgB,int nSize)
{  
	int cLUT[256],i,B;
	for(B=1;B<256;B++) cLUT[B]=(1<<22)/B;
	for(i=0;i<nSize;i++) 
	{
		B=*(pGryImgB+i);
		if (B)  *(pGryImgA+i) = (*(pGryImgA+i)*cLUT[B])>>22;
		else *(pGryImgA+i)=255;
	}
	return;
}


void main()
{
	clock_t t1,t2;
	int width,height;
	int nTstTimes=10;

	BYTE *pGryImgA=RmwRead8BitBmpFile2Img("GryImgA.bmp",&width,&height);
	BYTE *pGryImgB=RmwRead8BitBmpFile2Img("GryImgB.bmp",&width,&height);

	t1=clock();
	for(int i=0;i<nTstTimes;i++) 
	{
		RmwGryImageADivB_Slow(pGryImgA,pGryImgB,width*height);
		//RmwGryImageADivB_Fast(pGryImgA,pGryImgB,width*height);
	}
	t2=clock();
	printf("Time Consuming = %.3lf ms\n",1.0*(t2-t1)/nTstTimes);

	BYTE *pGryImgC=RmwRead8BitBmpFile2Img("GryImgA.bmp",&width,&height);
	RmwGryImageADivB_Fast(pGryImgC,pGryImgB,width*height);
	RmwWrite8BitImg2BmpFile(pGryImgC,width,height,"GryImgC.bmp");

	delete pGryImgA;
	delete pGryImgB;
	delete pGryImgC;
	return;
}
