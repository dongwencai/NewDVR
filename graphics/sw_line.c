
extern gdc;
int drawLine(POINT_S begin_s,POINT_S end_s,U16 u16Color,int nLineWidth)
{
	int bx,by,ex,ey;
	bx=begin_s.x;
	by=begin_s.y;
	ex=end_s.x;
	ey=end_s.y;
	U32 nStride = gdc.pSzWin.nW;
	U32 *pPhyAddr=gdc.pWinFb->pMappedAddr;
	int ax,ay;
	ax=bx<ex?1:-1;
	ay=bx<ex?1:-1;
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
		int nx,ny;
		int nx,ny,k,cnt;
		k=(ABS(by-ey)<<10)/ABS(bx-ex);
		for(nx=bx;nx!=ex;nx +=ax)
		{
			ny=((k*(nx-bx)+512)>>10)+by;
			for(cnt=0;cnt<nLineWidth;cnt++)
				pPhyAddr[ny*nStride+nx+cnt]=u16Color;
		}
	}
	return 0;
}
