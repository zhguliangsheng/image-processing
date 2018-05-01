///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� �� 
//        1. ������������������˿�����������Ȩ������������ҵĿ�ļ�Ӧ������С�     
//        2. ���ڱ������Ǹ��˿����ģ����ɱ���ش��ڸ��ָ�����ȱ�ݻ�������Ա�������ý���Ӧ���ڿ����о��ͽ�ѧ�С�
//        3. ����������Ӧ����ʵ������ʱ�����䵼�µĸ��ֲ�����������˸Ų�����
//        4. �����ַ����ļ����뱣����Щ���֡�
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

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   �߽紦��
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwSetImageBoundary(BYTE *BinImg,int width,int height,BYTE Color)
{	BYTE *pCur1,*pCur2;
	int x,y;

	for(x=0,pCur1=BinImg,pCur2=BinImg+(height-1L)*width;x<width;x++) *(pCur1++)=*(pCur2++)=Color;
	for(y=0,pCur1=BinImg,pCur2=BinImg+width-1;y<height;y++,pCur1+=width,pCur2+=width) *pCur1=*pCur2=Color;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Gradient����
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwGradientGrayImage(BYTE *pGryImg,int width,int height,BYTE *pGrImg)
{ 
   int i,j,G1,G2,x2,y2;
   BYTE *pGry,*pGr,*pCurGry,*pCurGr;

   x2=width-1;
   y2=height-1;
   RmwSetImageBoundary(pGrImg,width,height,0);
   for(i=1,pGry=pGryImg+width,pGr=pGrImg+width;i<y2;i++,pGry+=width,pGr+=width)
   {  
	  for(j=1,pCurGry=pGry+j,pCurGr=pGr+j;j<x2;j++,pCurGry++)
	  {	
		G1=*pCurGry-*(pCurGry-1);
       	G2=*pCurGry-*(pCurGry-width);
		*(pCurGr++)=(int)sqrt((double)G1*G1+G2*G2);
	  }
   }
   return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Robert����
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwRobertGrayImage(BYTE *pGryImg,int width,int height,BYTE *pRbImg)
{ 
   int i,j,G1,G2,x2,y2;
   BYTE *pGry,*pRb,*pCurGry,*pCurRb;

   x2=width-1;
   y2=height-1;
   RmwSetImageBoundary(pRbImg,width,height,0);
   for(i=1,pGry=pGryImg+width,pRb=pRbImg+width;i<y2;i++,pGry+=width,pRb+=width)
   {  
	  for(j=1,pCurGry=pGry+j,pCurRb=pRb+j;j<x2;j++,pCurGry++)
	  {	
		G1=(*pCurGry)-*(pCurGry+width+1);
       	G2=(*(pCurGry+width))-*(pCurGry+1);
   		*(pCurRb++)=max(abs(G1),abs(G2));
	  }
   }
   return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Sobel����
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwSobelGrayImage(BYTE *pGryImg,int width,int height,BYTE *pSbImg)
{ 
   int i,j,G1,G2,x2,y2;
   BYTE *pGry,*pSb,*pCurGry,*pCurSb;

   x2=width-1;
   y2=height-1;
   RmwSetImageBoundary(pSbImg,width,height,0);
   for(i=1,pGry=pGryImg+width,pSb=pSbImg+width;i<y2;i++,pGry+=width,pSb+=width)
   {  
	  for(j=1,pCurGry=pGry+j,pCurSb=pSb+j;j<x2;j++,pCurGry++)
	  {	
		G1  = *(pCurGry-width-1)+ (*(pCurGry-width)*2) + *(pCurGry-width+1);
		G1 -= *(pCurGry+width-1)+ (*(pCurGry+width)*2) + *(pCurGry+width+1);

		G2  = *(pCurGry-1-width)+ (*(pCurGry-1)*2) + *(pCurGry-1+width);
		G2 -= *(pCurGry+1-width)+ (*(pCurGry+1)*2) + *(pCurGry+1+width);

   		*(pCurSb++)=min(255,abs(G1)+abs(G2));
	//	*(pCurSb++)=abs(G1)+abs(G2);
	  }
   }
   return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Prewitt����
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwGradientByPrewitt(BYTE *pGryImg,int width,int height,int halfx,int halfy,BYTE *pResImg)
{  int	wx=2*halfx+1;
   int	wy=2*halfy+1;
   int  endy=height-halfy;
   int 	endx=width-halfx;
   int  endy1,endx1,num;
   int x,y ,i,y1,x1,sum1,sum2,v0,v45,v90,v135;
   BYTE *pByte,*qByte,*pRes,*pCur;
   int buff_array[1024],*buff,*p,*q;
   
	if (wx*wy<=1024) buff = buff_array;
	else buff = new int [wx*wy]; 
	memset(pResImg,0,width*halfy); // halfy=3 ==> 0,1,2 ==> 3*width
	memset(pResImg+(endy-1)*width,0,width*halfy); // height=512 ==> 509,510,511 ==> 3*width
    for(y=halfy,pRes=pResImg+y*width;y<endy;y++,pRes+=width)  
	{   endy1=y+halfy;
	    for(x=0,pCur=pRes+x;x<halfx;x++) *(pRes++)=0;
	    for(x=width-1,pCur=pRes+x;x>=endx;x--) *(pRes--)=0;
		for(x=halfx,pCur=pRes+x;x<endx;x++)
		{  
		   i=0;
		   endx1=x+halfx;
		   for(y1=y-halfy,pByte=pGryImg+y1*width;y1<=endy1;y1++,pByte+=width)
		     for(x1=x-halfx,qByte=pByte+x1;x1<=endx1;x1++) buff[i++]=*(qByte++);
		   // 0  edge in buff
		   num=halfy*wx;
		   for(i=0,p=buff,sum1=0;i<num;i++) sum1+=*(p++);
		   for(i=0,p=buff+(halfy+1)*wx,sum2=0;i<num;i++) sum2+=*(p++);
		   if (sum1>sum2) v0=sum1-sum2;
		   else v0=sum2-sum1;
		   // 90 edge in buff
		   for(y1=0,sum1=sum2=0,p=buff;y1<wy;y1++,p+=wx)
		   { for(x1=0,q=p;x1<halfx;x1++) sum1+=*(q++);
		     for(x1=halfx+1,q=p+x1;x1<wx;x1++) sum2+=*(q++);
		   }
		   if (sum1>sum2) v90=sum1-sum2;
		   else v90=sum2-sum1;
		   // 45 edge in buff
		   for(y1=0,sum1=sum2=0,p=buff;y1<wy;y1++,p+=wx)
		   { for(x1=0,q=p;x1<wy-y1-1;x1++) sum1+=*(q++);
		     for(x1=wy-y1,q=p+x1;x1<wx;x1++) sum2+=*(q++);
		   }
		   if (sum1>sum2) v45=sum1-sum2;
		   else v45=sum2-sum1;
		   // 135 edge in buff
		   for(y1=0,sum1=sum2=0,p=buff;y1<wy;y1++,p+=wx)
		   { for(x1=0,q=p;x1<y1;x1++) sum1+=*(q++);
		     for(x1=y1+1,q=p+x1;x1<wx;x1++) sum2+=*(q++);
		   }
		   if (sum1>sum2) v135=sum1-sum2;
		   else v135=sum2-sum1;
		   // true value
		   *(pCur++)=min(255,max(max(v0,v90),max(v45,v135)));
		   //*(pCur++)=max(max(v0,v90),max(v45,v135));
		}
	}
    if (wx*wy>1024)	delete buff;
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    ������
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwShenJunEdge(BYTE *pGryImg,BYTE *pTmpImg,int width,int height,double alpha,BYTE *pResImg) 
{ 	
	int i,x,y,j,pointer,size;
	double gLUT[512],*LUT;
    BYTE *cur,*pre,*pBuf1,*pBuf2,BLUT[256];

	alpha=min(0.99,alpha);
	alpha=max(0.01,alpha);
	// 1. step.1---------------------Shen Filter Image--------------------------------------//
	// init LUT
	for(i=0;i<512;i++) gLUT[i]=alpha*(i-256);//������ұ�
	LUT=gLUT+256;
    // copy image
    memcpy(pResImg,pGryImg,width*height);
    memcpy(pTmpImg,pGryImg,width*height);
	// pass One : p1(i,j)=p1(i,j-1)+a*[p(i,j)-p1(i,j-1)];
    for(y=0,pointer=y*width;y<height;y++,pointer+=width)
      for(x=1,cur=pTmpImg+pointer+x,pre=cur-1;x<width;x++,cur++,pre++) 
		  *cur=(BYTE)(*pre+LUT[*cur-*pre]);
    // pass two : p2(i,j)=p2(i,j+1)-a*[p1(i,j)-p2(i,j+1)];
    for(y=0,pointer=y*width;y<height;y++,pointer+=width)
      for(x=width-2,cur=pTmpImg+pointer+x,pre=cur+1;x>=0;x--,cur--,pre--) 
		  *cur=(BYTE)(*pre+LUT[*cur-*pre]);
    // passs three: p3(i,j)=p3(i-1,j)+a*[p2(i,j)-p3(i-1,j)];
    for(x=0;x<width;x++)
     for(y=1,cur=pTmpImg+y*width+x,pre=cur-width;y<height;y++,cur+=width,pre+=width)
	   *cur=(BYTE)(*pre+LUT[*cur-*pre]);
    // pass four: p4(i,j)=p4(i+1,j)+a*[p3(i,j)-p4(i+1,j)];
    for(x=0;x<width;x++)
     for(y=height-2,cur=pTmpImg+y*width+x,pre=cur+width;y>=0;y--,cur-=width,pre-=width)
	   *cur=(BYTE)(*pre+LUT[*cur-*pre]);
	// 2. step.2---------------------Cross Zero points--------------------------------------//
	pBuf1=pResImg; // orignal image
	pBuf2=pTmpImg; // shen image
	size=width*height;
	for(pointer=0;pointer<size;pointer++,pBuf1++,pBuf2++) 
	{ if ( *pBuf1>*pBuf2 ) *pBuf1=255;
	  else *pBuf1=0;
	}
    // cross Zero?==>128
	for(i=1,pointer=i*width;i<height-1;i++,pointer+=width)
	{ 
	  pBuf1=pResImg+pointer;
	  for(j=1,pBuf1+=j;j<width-1;j++,pBuf1++)
	  { if (*pBuf1==0) continue;
	    if (*(pBuf1-1)==0||*(pBuf1+1)==0||*(pBuf1-width)==0||*(pBuf1+width)==0) *pBuf1=128;
	  }
	}
	// edge points(128) to 255,others to 0.
    for(i=0;i<256;i++) BLUT[i]=0; 
	BLUT[128]=255;
	for(pointer=0,pBuf1=pResImg+pointer;pointer<size;pointer++) *(pBuf1++)=BLUT[*pBuf1];
	// clear image boundary
    RmwSetImageBoundary(pResImg,width,height,0);
	return;
} 
 