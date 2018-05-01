#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "bmpFile.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     图像按行/按列访问
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwAccessImageByRow(BYTE *pGryImg,int height,int width)
{  
	BYTE *pCur,*pEnd;
	for(pCur=pGryImg,pEnd=pGryImg+height*width;pCur<pEnd;pCur++)
	{
		*pCur = 255 - *pCur;
	}
	return;
}
void RmwAccessImageByCol(BYTE *pGryImg,int height,int width)
{  
	BYTE *pCur,*pEnd;
	for(int x=0;x<width;x++)
	{
		for(pCur=pGryImg+x,pEnd=pCur+x+(height-1)*width;pCur<pEnd;pCur+=width)
		{
			*pCur = 255 - *pCur;
		}
	}
	return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     32幅图像求均值
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define  WIDTH    768
#define  HEIGHT   576
int gSum[WIDTH*HEIGHT];
void RmwAverageImageSequenceFast(BYTE *pSeqImg[],int imgSize,BYTE *pResImg)
{
    int i,j;
	BYTE *pCurImg;
	for(j=0;j<imgSize;j++) 	gSum[j] = 0 ;
	for(i=0;i<32;i++)
	{
	    pCurImg = pSeqImg[i];
		for(j=0;j<imgSize;j++) 	gSum[j] += *(pCurImg+j);
	}  
	for(j=0;j<imgSize;j++)  *(pResImg+j) = gSum[j]>>5;	
	return;  	
}

void RmwAverageImageSequenceSlow(BYTE *pSeqImg[],int imgSize,BYTE *pResImg)
{
    int i,j;
	BYTE *pCurImg = NULL;

	for(j=0;j<imgSize;j++) 	gSum[j] = 0;
	for(j=0;j<imgSize;j++)
	{ 			
		for(i=0;i<32;i++) gSum[j] += *(pSeqImg[i]+j);
	}  
	for(j=0;j<imgSize;j++)  *(pResImg+j) = gSum[j]>>5;
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
	int nTstTimes=100;

	BYTE *pGryImg=RmwRead8BitBmpFile2Img("GryImg.bmp",&width,&height);

	t1=clock();
	for(int i=0;i<nTstTimes;i++) 
	{
		//RmwAccessImageByRow(pGryImg,height,width); //0.780ms
		RmwAccessImageByCol(pGryImg,height,width);   //2.500ms
	}
	t2=clock();
	printf("Time Consuming = %.3lf ms\n",1.0*(t2-t1)/nTstTimes);

	delete pGryImg;
	return;
}


void test2()
{
	clock_t t1,t2;
	int width,height;
	int i,nTstTimes=100;

	BYTE *pSeqImg[32];
	for(i=0;i<32;i++) pSeqImg[i] = RmwRead8BitBmpFile2Img("GryImg.bmp",&width,&height);

	t1=clock();
	for(i=0;i<nTstTimes;i++) 
	{
		//RmwAverageImageSequenceFast(&pSeqImg[0],width*height,pSeqImg[0]); //45.760ms
		RmwAverageImageSequenceSlow(&pSeqImg[0],width*height,pSeqImg[0]); //68.900ms
	}
	t2=clock();
	printf("Time Consuming = %.3lf ms\n",1.0*(t2-t1)/nTstTimes);

	for(i=0;i<32;i++) delete pSeqImg[i];
	return;
}
void main()
{
	//test1();
	test2();
}

