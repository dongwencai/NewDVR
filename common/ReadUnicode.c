

#include "unicode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



        
FILE *prf = NULL;
FL_HEADER _fl_header;
DWORD g_dwCharInfo = 0;

int OpenFontFile(char *file)
{
    if((prf = fopen(file, "rb")) == NULL)
    {
        printf("Cann't open : %s\n", file);
        return 0;
    }
        
    return 1;
}

void ReadFontHeader(PFL_HEADER pfl_header)
{   
    fread(pfl_header, sizeof(FL_HEADER) -4, 1, prf);
    if(pfl_header->pData==NULL)
    {
        pfl_header->pData = (FL_SECTION_INFO *)malloc(pfl_header->nSection*sizeof(FL_SECTION_INFO));
     }
    
    fread(pfl_header->pData, pfl_header->nSection*sizeof(FL_SECTION_INFO), 1, prf);  	
}


DWORD ReadCharDistX(WORD wCode)
{   
    DWORD offset ;
    int   i;    	
    
    for(i = 0;i<_fl_header.nSection;i++)
    {
        if(wCode >= _fl_header.pData[i].First && wCode <= _fl_header.pData[i].Last)
            break;  	
    }
    if(i >= _fl_header.nSection)
    {
        g_dwCharInfo = 0;
        return 0;   
    }
    
    offset = _fl_header.pData[i].OffAddr+ FONT_INDEX_TAB_LEN*(wCode - _fl_header.pData[i].First );
    fseek(prf, offset, SEEK_SET);
    fread(&g_dwCharInfo, sizeof(DWORD), 1, prf);

    return GET_FONT_WIDTH(g_dwCharInfo);
}


int ReadCharDotArray(WORD wCode, BYTE *fontArray, WORD *bytesPerLine)
{   

    *bytesPerLine= (WORD)((GET_FONT_WIDTH(g_dwCharInfo))+7)/8;  

    if(g_dwCharInfo > 0)
    {   	
        DWORD nDataLen = *bytesPerLine * _fl_header.YSize;      	
        DWORD  dwOffset = GET_FONT_OFFADDR(g_dwCharInfo);
        
        fseek(prf, dwOffset, SEEK_SET);
        fread(fontArray, nDataLen, 1, prf); 
        
        return 1;
    }
    
    return 0;
}
int ReadUnicodeInfo()
{
    
}

int GetFontInfo(PCHARINFO pCharInfo)
{
    pCharInfo->bWordWidth=ReadCharDistX(pCharInfo->wCode);
    ReadCharDotArray(pCharInfo->wCode,pCharInfo->bWordImage,&pCharInfo->bWordStride);
    pCharInfo->bWordHeight=_fl_header.YSize;
}
void FontInit(void)
{
    char fontfile[] = "arialuni16_CP1252.bin";
    WORD wCode = 0x21;  
    CHARINFO wordInfo;
    wordInfo.wCode=wCode;
    if(OpenFontFile(fontfile))
    {
        int i = 0, j = 0;
        int nWidth = 0;
        WORD bytesPerLine = 0;
        BYTE buf[MAX_FONT_SIZE * (MAX_FONT_SIZE/8)];
        memset(&_fl_header, 0, sizeof(FL_HEADER));
        ReadFontHeader(&_fl_header);
        GetFontInfo(&wordInfo);
        {
            printf("bytesPerLine = %d\n", wordInfo.bWordStride);
            for(i = 0; i < wordInfo.bWordHeight; i++)
        	{
                for(j = 0; j < wordInfo.bWordStride; j++)
                    printf("%x, ", wordInfo.bWordImage[i * wordInfo.bWordStride + j]);
        		
                printf("\n");
        	}
        }

    }
}
