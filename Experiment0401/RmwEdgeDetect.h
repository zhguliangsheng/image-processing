///////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

#ifndef RMW_EDGE_DETECTION
#define RMW_EDGE_DETECTION
 
void RmwSetImageBoundary(BYTE *BinImg,int width,int height,BYTE Color);
void RmwGradientGrayImage(BYTE *pGryImg,int width,int height,BYTE *pGrImg);
void RmwRobertGrayImage(BYTE *pGryImg,int width,int height,BYTE *pRbImg);
void RmwSobelGrayImage(BYTE *pGryImg,int width,int height,BYTE *pSbImg);
void RmwGradientByPrewitt(BYTE *pGryImg,int width,int height,int halfx,int halfy,BYTE *pResImg);
void RmwShenJunEdge(BYTE *pGryImg,BYTE *pTmpImg,int width,int height,double alpha,BYTE *pResImg); 

#endif
