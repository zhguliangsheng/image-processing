#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "bmpFile.h"
#include "RmwEdgeDetect.h"
#include "RmwChainCode.h"

void test_trace_and_fill()
{   
	BYTE *pBinImg,*pBin;
	int width,height,imgSize;
	int i,x,y,Cur,Pre,length,Area,Perimeter,PointsNum;
	BYTE *pDilationImg,*pFillImg,*pCode,*pErosionImg;
	
	// open file
	pBinImg=RmwRead8BitBmpFile2Img("rice_Bin_org.bmp",&width,&height);
	if (!pBinImg) { printf("\nfopen err!\n"); return; }

    imgSize=width*height;
	pDilationImg = new BYTE [imgSize];
	pErosionImg = new BYTE [imgSize];
	pFillImg = new BYTE [imgSize];
	pCode = new BYTE [imgSize];
	memset(pFillImg,0,sizeof(BYTE)*imgSize);
	// Traceing Contours
	RmwSetImageBoundary(pBinImg,width,height,0);
	for(y=1,pBin=pBinImg+y*width;y<height;y++)
	{
        for(x=0;x<width;x++,pBin++)
		{  
			Cur=*pBin;
			Pre=*(pBin-1);
			if (Cur==Pre) continue;
			// 链码跟踪 
			length=0;
			if ((Cur==255)&&(Pre<10))  //外轮廓
			{
				length=RmwTraceContour(pBinImg,width,x,y,pCode,imgSize,true);
				RmwAreaAndPerimeter(pCode,length,&Area,&Perimeter);
				RmwRealAreaAndPerimeter(pCode,length,&PointsNum,&Perimeter);
				printf("外轮廓包含的面积为=%05d,周长为=%5d,所包含的像素点数为=%-5d\n",Area,Perimeter,PointsNum);
				if (PointsNum>300) RmwFillingContour(pFillImg,width,x,y,pCode,length,true,1,100);
				else RmwFillingContour(pFillImg,width,x,y,pCode,length,true,1,255);
			}
			else  //内轮廓
			{
				if ((Cur==0)&&(Pre>250)) 
				{
					length=RmwTraceContour(pBinImg,width,x-1,y,pCode,imgSize,false);
					RmwAreaAndPerimeter(pCode,length,&Area,&Perimeter);
					RmwRealAreaAndPerimeter(pCode,length,&PointsNum,&Perimeter);
                    printf("内轮廓包含的面积为=%05d,周长为=%5d,所包含的像素点数为=%-5d\n",Area,Perimeter,PointsNum);
					RmwFillingContour(pFillImg,width,x-1,y,pCode,length,false,1,255); //孔全部填成白色
				}
			}
		}
	}
	// 得到3x3的膨胀/腐蚀结果
	for(i=0;i<imgSize;i++) 
	{
		// 得到3x3的膨胀结果
		if (*(pBinImg+i)) *(pDilationImg+i)=255; 
		else *(pDilationImg+i)=0;

		// 得到3x3的腐蚀结果
		if (*(pBinImg+i)!=255) *(pErosionImg+i)=0;
		else *(pErosionImg+i)=255;
	}
	// return
	RmwWrite8BitImg2BmpFile(pDilationImg,width,height,"rice_dilationImg.bmp");
	RmwWrite8BitImg2BmpFile(pErosionImg,width,height,"rice_erosionImg.bmp");
	RmwWrite8BitImg2BmpFile(pFillImg,width,height,"rice_select.bmp");
	delete pFillImg;
	delete pDilationImg;
	delete pErosionImg;
	delete pCode;
	return;
}

void test_select_max()
{
	int width,height;

	// open file
	BYTE *pBinImg =  RmwRead8BitBmpFile2Img("Dune_Prewitt5x5_bin_org.bmp",&width,&height);
	if (!pBinImg) return;

	// select max
	BYTE *pBufImg =  new BYTE [width*height];
	struct ContourStruct *p,*q,*pContours;
	p=q=pContours=RmwGetContours(pBinImg,pBufImg,width,height,50,50,1);
	int max=0;
	while(p)
	{
		if (p->Area>max)
		{
			max=p->Area;
			q=p;
		}
		p=p->Next;
	}

	// mark
	memset(pBufImg,0,width*height);
	RmwMarkChainByColor(pBufImg,width,q->x0,q->y0,q->Chain,q->ChainLength,255);
    RmwWrite8BitImg2BmpFile(pBufImg,width,height,"Dune_Prewitt5x5_bin_line.bmp");

	//return
	RmwFreeContourStruct(pContours);
	delete pBinImg;
	delete pBufImg;
	return;
}

void main()
{
	//test_trace_and_fill();
	test_select_max();
 	return;
}
