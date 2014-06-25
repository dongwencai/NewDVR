#include "hi_common.h"
#include "sw_fb.h"

DC_S gdc;

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
	pPhyAddr=gdc.pHdOsdFb->pMappedAddr;

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
