void GUI__IntersectRect(PRECT pDest, const PRECT pr0)
{
    if (pDest->lTop.s32X< pr0->lTop.s32X)
    {
        pDest->x0 = pr0->x0;
    }
    if (pDest->lTop.s32Y< pr0->lTop.s32Y)
    {
        pDest->y0 = pr0->y0;
    }
    if (pDest->rBottom.s32X > pr0->rBottom.s32X)
    {
        pDest->x1 = pr0->x1;
    }
    if (pDest->rBottom.s32Y > pr0->rBottom.s32Y) 
    {
        pDest->y1 = pr0->y1;
    }
}

