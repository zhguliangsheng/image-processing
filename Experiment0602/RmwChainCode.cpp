///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//File Name : rmwChainCode.cpp  
//Version   : 1.0 
//Author    : Ren Ming Wu, renmingwu@mail.njust.edu.cn
//            南京理工大学计算机系国家重点学科模式识别与智能系统,210094
// 声明 ： 
//        1. 本程序属于任明武个人开发，不经授权，不得用于商业目的及应用软件中。     
//        2. 由于本程序是个人开发的，不可避免地存在各种各样的缺陷或错误，所以本程序最好仅仅应用于科研研究和教学中。
//        3. 若将本程序应用在实际领域时，由其导致的各种不良后果，本人概不负责。
//        4. 如果你分发该文件，请保留这些文字。
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Caution:
// 1) If you do not get authorization from Mingwu Ren, 
//    this lib CAN NOT be used for commerce.
// 2) It is only developed by individual, so the errors can not be avioded. 
//    I strongly suggest that it should only be used for research and education.
// 3) If you find errors, you can tell me. E-Mail: renmingwu@mail.njust.edu.cn
// 4) If you redistribute this lib, please keep these words.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <memory.h>

#include "RmwChainCode.h"
#include "RmwEdgeDetect.h"
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 链码系列函数-跟踪和填充
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int RmwTraceContour(BYTE *Img,int width,int x,int y,BYTE *codebuf,int bufsize,bool isOut)
{ 
  OFFSETX;OFFSETY;
  static int firsttestcode[8]={7, 7, 1, 1, 3, 3, 5, 5};
  int offsetp[8];
  BYTE *pc0,*pci,*pc;
  int i,k,testcodei,curc,fc0,fck,codenum;

    for(i=0;i<8;i++) offsetp[i]=offsety[i]*width+offsetx[i];
	pc=pc0=Img+y*width+x; codenum=0;
	if (isOut) {*(pc-1)=1;curc=7;fc0=5;}
	else {curc=3;fc0=1;*(pc+1)=1;}
	do{ *pc=254;
		//for(i=0,testcodei=firsttestcode[curc];i<7;i=i+1,testcodei=(testcodei+1)%8)
		for(i=0,testcodei=firsttestcode[curc];i<7;i=i+1,testcodei=(testcodei+1)&0x07)
	    {  pci=pc+offsetp[testcodei];
           if (*pci<2) *pci=1;
		   else 
		   {  curc=testcodei;
			  if (codenum<bufsize-1) codebuf[codenum++]=testcodei;
		      if (pci==pc0)
			  {  
				 //for(k=0,fck=fc0; (fck!=(testcodei+4)%8)&&(k<7); k=k+1,fck=(fck+1)%8);
			     for(k=0,fck=fc0; (fck!=((testcodei+4)&0x07))&&(k<7); k=k+1,fck=((fck+1)&0x07));
                 //for(i=k+1,fck=(fck+1)%8; i<7; i=i+1,fck=(fck+1)%8)
                 for(i=k+1,fck=(fck+1)&0x07; i<7; i=i+1,fck=(fck+1)&0x07)
				 {  pci=pc0+offsetp[fck];
					if (*pci<2) *pci=1;
				    else
					{ curc=fck; 
					  if (codenum<bufsize-1)codebuf[codenum++]=fck; 
				      break;
					}
				 }
			  }
		      break;
		   }
		}
        pc=pci;
	}while(i<7);
    return(codenum);
}

int RmwFillingContour(BYTE *pImg,int width,int x0,int y0,const BYTE *code,int N,bool fillEdge,BYTE nonc,BYTE fillc)
{   static int angle[8][8]={ 0, 1, 2, 3, 4,-1,-1,-1,
                            -1, 0, 1, 2, 3, 4,-1,-2, 
                            -1,-1, 0, 1, 2,	3, 4,-1,
                            -1,-2,-1, 0, 1,	2, 3, 4,
                             4,-1,-1,-1, 0,	1, 2, 3,
                             3, 4,-1,-2,-1, 0, 1, 2,
                             2, 3, 4,-1,-1,-1, 0, 1,
                             1, 2, 3, 4,-1,-2,-1, 0
	                       };            
	OFFSETX;OFFSETY;
	BYTE *pbegin,*pcur,*ptst,buff[4096],*data_buff;
    int i,offsetp[8],shiftc,direction,g,used_color[256],k,startc,preCode;

	// step.0-----init,skipc and shiftc--------------------//
	pbegin=pImg+y0*width+x0;
	for(i=0;i<8;i++) offsetp[i]=offsety[i]*width+offsetx[i];
    // shiftc
    for(i=0;i<256;i++) used_color[i]=0;
	used_color[nonc]=used_color[fillc]=1;
	for(i=0,k=0;i<256;i++)
	{ if (used_color[i]==0) k++;
	  else k=0;
	  if (k==5) { shiftc=i-2; break; }
	}
	for(i=-2;i<=2;i++) used_color[shiftc+i]=1;
	for(i=0;i<256;i++)
		if (used_color[i]==0) { startc=i; break; }

    // step.1-----backup ----------------------------------//
	if (!fillEdge)
	{   if (N<=4096) data_buff=buff;
	    else data_buff = new BYTE [N];
		for(i=0,pcur=pbegin;i<N;pcur+=offsetp[code[i]],i++) data_buff[i]=*pcur;
	}

    // step.2-----sum offsety of reach and leave code------//
    for(i=0,pcur=pbegin;i<N;pcur+=offsetp[code[i]],i++) *pcur=shiftc;
 	for(i=0,direction=0,pcur=pbegin,preCode=code[N-1];i<N;pcur+=offsetp[code[i]],i++) 
	{	(*pcur)=(*pcur)+offsety[preCode]+offsety[code[i]];
		direction+=angle[preCode][code[i]];
		preCode=code[i];
	}
	direction/=8;

	// step.3-----classify contour points-----------------//
    for(i=0,pcur=pbegin;i<N;pcur+=offsetp[code[i]],i++) 
	{   g=*pcur;
		if ( (g==shiftc+1)||(g==shiftc+2) ) *pcur=startc;
		else if ( (g==shiftc-1)||(g==shiftc-2) ) *pcur=nonc;
	}

    // step.4-----filling interior------------------------//
    for(i=0,pcur=pbegin;i<N;pcur+=offsetp[code[i]],i++) 
	{   if (*pcur!=startc) continue;
	    ptst=pcur;
		while (*ptst!=nonc) 
		{ *ptst=fillc;
		  ptst+=direction; 
		} 
		*pcur=nonc; // avoid repeatly filling
	}
    // step.5-----filling boundary------------------------//
	if (!fillEdge)
	{   for(i=0,pcur=pbegin;i<N;pcur+=offsetp[code[i]],i++) *pcur=data_buff[i];
	    if (N>4096) delete data_buff;
	}
    else for(i=0,pcur=pbegin;i<N;pcur+=offsetp[code[i]],i++) *pcur=fillc;

    // step.6-----return----------//
	return direction;
}

struct ContourStruct *RmwGetContours(BYTE *pBinImg,BYTE *pImgBuf,int width,int height,int MiniP,int MiniS,int Out1In2ALL3)
{  
  //   得到所有的轮廓
  int ImgSize,i,y,x,length,Area,Perimeter; 
  BYTE *Chain,*pCode=pImgBuf;
  BYTE *pBin,Cur,Pre;
  struct ContourStruct *Result,*Tail,*NewObj;

    // step.1--------Init
    Result=NULL;
    ImgSize = width*height;
	//step.2--------目标跟踪
	RmwSetImageBoundary(pBinImg,width,height,0); //为了跟踪，必须清0
	for(y=1,pBin=pBinImg+y*width;y<height;y++)
        for(x=0;x<width;x++,pBin++)
		{  Cur=*pBin;
		   Pre=*(pBin-1);
		   if (Cur==Pre) continue;
           // 链码跟踪 
		   length=0;
		   if ((Cur==255)&&(Pre<10)) length=RmwTraceContour(pBinImg,width,x,y,pCode,ImgSize,true);
           else if ((Cur==0)&&(Pre>250)) length=RmwTraceContour(pBinImg,width,x-1,y,pCode,ImgSize,false); 
           // 链码筛选
		   if ( (length==0)||(length<MiniP) ) continue;
		   if ( (Out1In2ALL3==1)&&(Cur==0) ) continue;
		   if ( (Out1In2ALL3==2)&&(Cur==255) ) continue;
		   RmwRealAreaAndPerimeter(pCode,length,&Area,&Perimeter);
		   if ( (Area<MiniS)||(Perimeter<MiniP) ) continue;
		   // 纪录新轮廓
		   Chain=new BYTE [length];
		   if (Chain==NULL) continue;
		   for(i=0;i<length;i++) Chain[i]=pCode[i];
		   NewObj=(struct ContourStruct *) new BYTE [sizeof(struct ContourStruct)];
		   if (NewObj==NULL) { delete Chain; continue;}
		   if (Cur==255) 
		   {  NewObj->IsOutContour=true;
		      NewObj->x0=x;
		      NewObj->y0=y;
		   }
		   else 
		   {  NewObj->IsOutContour=false;
		      NewObj->x0=x-1;
		      NewObj->y0=y;
		   }
           NewObj->Area = Area;
		   NewObj->Perimeter= Perimeter;
		   NewObj->Left=NewObj->Right=NewObj->Top=NewObj->Bottom=0;
		   NewObj->UsedDown=NewObj->UsedDown=0;
		   NewObj->ChainLength=length;
		   NewObj->Chain=Chain;
		   NewObj->Next=NULL;
		   // 追加
		   if (Result==NULL) Result=Tail=NewObj;
		   else
		   { Tail->Next=NewObj; 
		     Tail=Tail->Next;
		   }
		}
	// step.3---------return---------------------------//
    //RmwThreshold(pBinImg,width,height,50);
	return Result;
}

void RmwFreeContourStruct(struct ContourStruct *all_Contour)
{   
	struct ContourStruct *q,*p=all_Contour;
    while(p!=NULL) { q=p->Next; delete p->Chain; delete p; p=q; }
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 链码系列函数-特征和描述
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwRealAreaAndPerimeter(BYTE *code,int N,int *Area,int *Perimeter)
{
    static int Label[8]={0,1,0,1,0,1,0,1};
	static int PointKindLUT[8][8]={1,1,1,3,3,2,2,1,//0
		/*[inCode][outCode]*/        1,1,1,3,3,3,2,1,//1
		/*0-下边界   1-上边界*/      2,2,2,0,0,0,0,2,//2
		/*2-无用边界 3-尖点*/        2,2,2,0,0,0,0,3,//3
		3,2,2,0,0,0,0,3,//4
		3,3,2,0,0,0,0,3,//5
		1,1,1,2,2,2,2,1,//6
		1,1,1,3,2,2,2,1 //7
	};
    static int PrjY_SignLUT[3]={-1,1,0}; //符号表
    static int Prj_YMend[3]={0,1,1}; //修正表
    int in_code,out_code,pointkind,i,s,y,N01[2];
	OFFSETX;OFFSETY;
	
	N01[0]=N01[1]=y=s=0;
	for(i=0;i<N;i++)
	{  	
		in_code=code[(i-1+N)%N];
		out_code=code[i];
		pointkind=PointKindLUT[in_code][out_code];
		if(pointkind==3) s+=1;
		else s+=PrjY_SignLUT[pointkind]*(y+Prj_YMend[pointkind]);
		y+=offsety[out_code];
		N01[Label[out_code]]++;
	}
    *Perimeter=(int)(N01[0]*1.0+N01[1]*sqrt(2.0)+0.5);
	*Area=abs(s);
	return;
}

void RmwAreaAndPerimeter(BYTE *Code,int N,int *Area,int *Perimeter)
{  
	OFFSETX;OFFSETY;
   static int Label[8]={0,1,0,1,0,1,0,1};
   int i,c,y,N01[2];
   double area;
   
   area=N01[0]=N01[1]=y=0;
   for(i=0;i<N;i++)
   { 
	   c=Code[i];
	   area+=offsetx[c]*(y+0.5*offsety[c]); 
	   y+=offsety[c];N01[Label[c]]++;
   }
   *Perimeter=(int)(N01[0]*1.0+N01[1]*sqrt(2.0)+0.5);
   *Area=(int)(fabs(area)+0.5+(*Perimeter)/2);
   return;
}

void RmwMarkChainByColor(BYTE *img,int width,int x,int y,BYTE *Code,int N,BYTE Color)
{  
   OFFSETX;OFFSETY;
   int i,offsetp[8];
   BYTE *pCur;

   *(img+y*width+x)=Color; // 为了能够处理不闭合的轮廓
   for(i=0;i<8;i++) offsetp[i]=offsety[i]*width+offsetx[i];
   for(pCur=img+y*width+x,i=0;i<N;i++) 
   {  pCur+=offsetp[Code[i]];
      *pCur=Color;
   }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 灵活运用1: 用作边缘选择
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwEdgeDeleteNoiseAndCrossPoints(BYTE *pEdgeImg,int width,int height,int nMaxNeighbors)
{   // 对于边缘图像而言,直线应该仅有两个邻居.
	// 若多于两个则为多条直线的交点或者是噪声,从而去掉.
	// 该函数调用后,使用链码跟踪,选取周长大于一定值者,即为线段
	BYTE *pCur,*pEnd=pEdgeImg+width*height;
	OFFSETX;OFFSETY;
	int offsetp[8],k,num;

	for(k=0;k<8;k++) offsetp[k]=offsety[k]*width+offsetx[k];
	RmwSetImageBoundary(pEdgeImg,width,height,0);
	for(pCur=pEdgeImg;pCur<pEnd;pCur++)
	{
		if (!(*pCur)) continue;
		for(num=0,k=0;k<8;k++)
		{
			if (*(pCur+offsetp[k]))
			{
				num++;
				if (num>nMaxNeighbors) { *pCur=128; break; }
			}
		}
	}
	
	for(pCur=pEdgeImg;pCur<pEnd;pCur++)
	{
		if (*pCur==128) *pCur=0;
	}
	
	return;
}

void RmwEdgeDeleteNoiseByLength(BYTE *pEdgeImg,int width,int height,int minLength,BYTE *pResImg)
{
    struct ContourStruct *p,*pContours;
	
	p=pContours=RmwGetContours(pEdgeImg,pResImg,width,height,minLength,0,1);
	memset(pResImg,0,width*height);
	while(p)
	{
		RmwMarkChainByColor(pResImg,width,p->x0,p->y0,p->Chain,p->ChainLength,255);
		p=p->Next;
	}
    RmwFreeContourStruct(pContours);
	return;
}
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 灵活运用2: 3x3的膨胀和腐蚀
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
