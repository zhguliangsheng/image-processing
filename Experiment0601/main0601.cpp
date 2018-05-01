#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "bmpFile.h"
#include "RmwEdgeDetect.h"
#include "RmwChainCode.h"

void RmwAreaAndPerimeter1(BYTE *Code,int N,double *Area,double *Perimeter)
{  
   OFFSETX;OFFSETY;
   static int Label[8]={0,1,0,1,0,1,0,1};
   int i,c,y,N01[2];
   double area;
   
   area=N01[0]=N01[1]=y=0;
   for(i=0;i<N;i++){ c=Code[i];area+=offsetx[c]*(y+0.5*offsety[c]); y+=offsety[c];N01[Label[c]]++;}
   *Perimeter=N01[0]*1.0+N01[1]*sqrt(2.0);
   *Area=fabs(area);
   return;
}

void Test()
{
	//unsigned char tstChain[]={7,6,6,6,0,0,0,0,7,7,2,4,3,3,1,3,2,4,4,4,4};
	//int N=21;

	unsigned char tstChain[]={1,0,7,5,7,5,4,3,2,2};
	int N=10;

	double Area,Perimeter;
	RmwAreaAndPerimeter1(tstChain,N,&Area,&Perimeter);
	printf("%.3lf %.3lf",Area,Perimeter);
	return;
}
void main()
{
	//Test();
	//return;

	BYTE *pOrgImg;
	int width,height;

	// open file
	pOrgImg =  RmwRead8BitBmpFile2Img("shiplock.bmp",&width,&height);
	if (!pOrgImg) return;

	// edge detection
	BYTE *pTmpImg =  new BYTE [width*height];
	BYTE *pShenJunImg =  new BYTE [width*height];
	RmwShenJunEdge(pOrgImg,pTmpImg,width,height,0.3,pShenJunImg); //Éò¿¡Ëã×Ó
	RmwWrite8BitImg2BmpFile(pShenJunImg,width,height,"shiplock_edge.bmp");

	// edge filter 1
	RmwEdgeDeleteNoiseAndCrossPoints(pShenJunImg,width,height,2);
	RmwWrite8BitImg2BmpFile(pShenJunImg,width,height,"shiplock_edge_filter1.bmp");

	// edge filter 2
	BYTE *pResImg =  new BYTE [width*height];
	RmwEdgeDeleteNoiseByLength(pShenJunImg,width,height,20,pResImg);
	RmwWrite8BitImg2BmpFile(pResImg,width,height,"shiplock_edge_filter2_20.bmp");

	// return
	delete pOrgImg;
	delete pShenJunImg;
	delete pTmpImg;
	delete pResImg;
	return;
}
