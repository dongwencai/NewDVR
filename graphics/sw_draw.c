#include "hi_common.h"
#include "sw_fb.h"
#include "unicode.h"
#include "sw_fb.h"
#include "window.h"
#include <wchar.h>
#include <stdlib.h>
DC_S gdc;
int drawRectangle(POINT_S leftTop_s,POINT_S rightBottom_s,U16 u16Color,int nLineWidth);
int drawLine(POINT_S begin_s,POINT_S end_s,U16 u16Color,int nLineWidth)
{
	int ax,ay,bx,by,ex,ey;
	S32 nStride;
	U16 *pPhyAddr=NULL;
	bx=begin_s.s32X;
	by=begin_s.s32Y;
	ex=end_s.s32X;
	ey=end_s.s32Y;
	ax=(bx<ex?1:-1);
	ay=(by<ey?1:-1);
	nStride =gdc.pSzWin.nW;
	pPhyAddr=gdc.pWinFb->pMappedAddr;

	if(bx==ex)				//Ð±ÂÊ²»´æÔÚ
	{
		int nx,ny;
		for(ny=by;ny<ey;ny++)
		{
			for(nx=bx;nx<bx+nLineWidth;nx++)
			{
				pPhyAddr[ny*nStride+nx]=u16Color;
			}
		}
	}
	else
	{
		int nx,ny,k,cnt;
		k=(ABS(by-ey)<<10)/ABS(bx-ex);
		if(k>1024)
		{
			for(ny=by;ny!=ey;ny +=ay)
			{
				nx=bx+((((ny-by)<<20)/k+512)>>10);
				for(cnt=0;cnt<nLineWidth;cnt++)
				{
					pPhyAddr[ny*nStride+nx+cnt]=u16Color;
				}
			}
		}
		else
		{
			for(nx=bx;nx!=ex;nx +=ax)
			{
				ny=by+((k*(nx-bx)+512)>>10);
				for(cnt=0;cnt<nLineWidth;cnt++)
				{
					pPhyAddr[(ny+cnt)*nStride+nx]=u16Color;
				}
			}
		}
	}
	return 0;
}

int drawRectangle(POINT_S leftTop_s,POINT_S rightBottom_s,U16 u16Color,int nLineWidth)
{
	POINT_S rightTop_s,leftBottom_s;
	rightTop_s.s32X=rightBottom_s.s32X;
	rightTop_s.s32Y=leftTop_s.s32Y;
	leftBottom_s.s32X=leftTop_s.s32X;
	leftBottom_s.s32Y=rightBottom_s.s32Y;
	drawLine(leftTop_s,rightTop_s,u16Color,nLineWidth);
	drawLine(leftTop_s,leftBottom_s,u16Color,nLineWidth<<1);
	drawLine(rightTop_s,rightBottom_s,u16Color,nLineWidth<<1);
	rightBottom_s.s32X +=(nLineWidth<<1);
	drawLine(leftBottom_s,rightBottom_s,u16Color,nLineWidth);
}

int drawAWord(PFRAMEBUFFER fb,RECT rect,PCHARINFO charInfo,U16 fg)
{
    int i,j;
    S32 nStride;
    U16 *pPhyAddr=NULL;
    U32 u32Sw,u16Ew;
    U32 u16WordPos;
    nStride =gdc.pSzWin.nW;
    pPhyAddr=fb->pMappedAddr;
    u32Sw=rect.lTop.s32X+rect.lTop.s32Y*nStride;
    //u16Ew=rect.lTop.s32Y;
    for(i=1;i<=charInfo->bWordHeight&&(rect.lTop.s32Y+i)<rect.rBottom.s32Y;i++)
    {
        u16WordPos=u32Sw+i*nStride;
        for(j=0;(j<charInfo->bWordStride)&&((rect.lTop.s32X+j*8)<rect.rBottom.s32X);j++)
        {
            unsigned char temp=0x80,cnt=0;
            U16    wCode=charInfo->bWordImage[i * charInfo->bWordStride + j];
            u16WordPos +=j*8;
            while(temp&&((u16WordPos+cnt)%nStride<rect.rBottom.s32X))
            {
                
                if(temp&wCode)
                {
                    pPhyAddr[u16WordPos+cnt]=fg;
                }
                //else
                temp >>=1;
                cnt ++;
            }
        }
    }
}
int drawStr(PFRAMEBUFFER fb,RECT rect,U16 *wChar,U16 fg,U16 bg)
{
    int cnt=0;
    CHARINFO charInfo={0};
    int len=wcslen(wChar);
    if(bg)
    {
        fillRectangle(rect.lTop,rect.rBottom,bg);
    }
    while((rect.rBottom.s32X-rect.lTop.s32X)>0&&cnt<len)
    {
        charInfo.wCode=wChar[cnt];
        GetFontInfo(&charInfo);
        drawAWord(fb,rect,&charInfo,fg); 
        rect.lTop.s32X +=charInfo.bWordWidth;
        cnt++;
    }
}
int drawCircle(PFRAMEBUFFER fb,RECT rect,POINT_S o,U16 r,U16 fg,U16 bg)
{
    S32 nStride;
    U16 *pPhyAddr=NULL;
    int nx,ny;
    nStride =gdc.pSzWin.nW;
    pPhyAddr=fb->pMappedAddr;
#define setPixel(x,y,color) ({pPhyAddr[(y)*nStride+(x)]= color;})
    for(nx=0;nx<=r;nx++)
    {
        POINT_S pos;
        ny=(((int)sqrt((r*r-nx*nx)<<10)+16)>>5);
        pos.s32X=o.s32X+nx;
        pos.s32Y=o.s32Y+ny;
        if(inArea(pos, rect))
            setPixel(pos.s32X, pos.s32Y, fg);
        pos.s32X=o.s32X-nx;
        if(inArea(pos, rect))
            setPixel(pos.s32X, pos.s32Y, fg);
        pos.s32Y=o.s32Y-ny;
        if(inArea(pos, rect))
            setPixel(pos.s32X, pos.s32Y, fg);
        pos.s32X=o.s32X+nx;
        if(inArea(pos, rect))
            setPixel(pos.s32X, pos.s32Y, fg);
    }
    for(ny=0;ny<=r;ny++)
    {
        POINT_S pos;
        nx=(((int)sqrt((r*r-ny*ny)<<10)+16)>>5);
        pos.s32X=o.s32X+nx;
        pos.s32Y=o.s32Y+ny;
        if(inArea(pos, rect))
            setPixel(pos.s32X, pos.s32Y, fg);
        pos.s32X=o.s32X-nx;
        if(inArea(pos, rect))
            setPixel(pos.s32X, pos.s32Y, fg);
        pos.s32Y=o.s32Y-ny;
        if(inArea(pos, rect))
            setPixel(pos.s32X, pos.s32Y, fg);
        pos.s32X=o.s32X+nx;
        if(inArea(pos, rect))
            setPixel(pos.s32X, pos.s32Y, fg);
    }
#undef setPixel
}