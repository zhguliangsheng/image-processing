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

#ifndef RMW_EDGE_DETECTION
#define RMW_EDGE_DETECTION
 
void RmwSetImageBoundary(BYTE *BinImg,int width,int height,BYTE Color);
void RmwGradientGrayImage(BYTE *pGryImg,int width,int height,BYTE *pGrImg);
void RmwRobertGrayImage(BYTE *pGryImg,int width,int height,BYTE *pRbImg);
void RmwSobelGrayImage(BYTE *pGryImg,int width,int height,BYTE *pSbImg);
void RmwGradientByPrewitt(BYTE *pGryImg,int width,int height,int halfx,int halfy,BYTE *pResImg);
void RmwShenJunEdge(BYTE *pGryImg,BYTE *pTmpImg,int width,int height,double alpha,BYTE *pResImg); 

#endif
