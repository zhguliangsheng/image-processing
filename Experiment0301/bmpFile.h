#ifndef BMP_FILE_H
#define BMP_FILE_H
typedef unsigned char BYTE;
BYTE *RmwRead8BitBmpFile2Img(const char * filename,int *width,int *height);
bool RmwWrite8BitImg2BmpFile(BYTE *pImg,int width,int height,const char * filename);

BYTE *RmwRead24BitBmpFile2Img(const char * filename,int *width,int *height);
bool RmwWrite24BitImg2BmpFile(BYTE *pImg,int width,int height,const char * filename);

#endif
