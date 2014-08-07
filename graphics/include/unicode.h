#ifndef __UNICODE_H__
#define __UNICODE_H__
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

typedef struct tagFlSectionInfo{
    WORD  First;        
    WORD  Last;         
    DWORD OffAddr;     
} FL_SECTION_INFO, *PFL_SECTION_INFO;

typedef struct tagFontLibHeader{
    BYTE    magic[4];
    DWORD   Size;            
    BYTE    nSection;
    BYTE    YSize;                 
    WORD    wCpFlag;
    char    reserved[4];
    FL_SECTION_INFO  *pData;
} FL_HEADER, *PFL_HEADER;

#define MAX_FONT_SIZE               	64
#define FONT_INDEX_TAB_LEN              4    //单个字符对应的字体检索信息长度为 4Byte  (b0~b25: 记录点阵信息的起始地址, b26~b31: 记录当前字符的象素宽度)
#define GET_FONT_WIDTH(charinfo)        (charinfo >> 26)
#define GET_FONT_OFFADDR(charinfo)  (charinfo & 0x3ffffff)
#define DB2UC(c1,c2)                    (c1 | (((U16)c2)<<8))

typedef struct{
    WORD wCode;
    BYTE bWordImage[MAX_FONT_SIZE * (MAX_FONT_SIZE/8)];
    BYTE bWordStride;
    BYTE bWordHeight;
    BYTE bWordWidth;
}CHARINFO,*PCHARINFO;

#endif
