#ifndef  RMW_CHAIN_CODE_H
#define  RMW_CHAIN_CODE_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���ݽṹ
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define OFFSETX static int offsetx[8]={1, 1, 0,-1,-1,-1, 0, 1}
#define OFFSETY static int offsety[8]={0,-1,-1,-1, 0, 1, 1, 1}
struct ContourStruct{
	bool IsOutContour; //true:������; false:������
	int x0,y0; //�����������
	int ChainLength; //���ĳ���
	BYTE *Chain;//�������飬��ʱ�뷽��8��ͨ��
	struct ContourStruct *Next; //ָ����һ��Ŀ��

	int Area,Perimeter; //������ܳ�����ֵ
	int Left,Right,Top,Bottom;//��Ӿ��Σ���ֵ
	int UsedDown,UsedUp; //��ʱ�����ñ�������ֵ
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ϵ�к���
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int RmwTraceContour(BYTE *Img,int width,int x,int y,BYTE *codebuf,int bufsize,bool isOut);
int RmwFillingContour(BYTE *pImg,int width,int x0,int y0,const BYTE *code,int N,bool fillEdge,BYTE nonc,BYTE fillc);
struct ContourStruct *RmwGetContours(BYTE *pBinImg,BYTE *pImgBuf,int width,int height,int MiniP,int MiniS,int Out1In2ALL3);
void RmwFreeContourStruct(struct ContourStruct *all_Contour);

void RmwRealAreaAndPerimeter(BYTE *code,int N,int *Area,int *Perimeter);
void RmwAreaAndPerimeter(BYTE *Code,int N,int *Area,int *Perimeter);
void RmwMarkChainByColor(BYTE *img,int width,int x,int y,BYTE *Code,int N,BYTE Color);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������1: ������Եѡ��
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RmwEdgeDeleteNoiseAndCrossPoints(BYTE *pEdgeImg,int width,int height,int nMaxNeighbors);
void RmwEdgeDeleteNoiseByLength(BYTE *pEdgeImg,int width,int height,int minLength,BYTE *pResImg);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������2: 3x3�����ͺ͸�ʴ
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
