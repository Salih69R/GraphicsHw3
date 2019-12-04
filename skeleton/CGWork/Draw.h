#ifndef DRAW_H
#define DRAW_H


#include <afxwin.h>

void MidPointDraw(int x1, int y1, int x2, int y2, int* bits, COLORREF color, int width, int height);
COLORREF RGBToBGR(COLORREF col);//used to switch the order because for some reason the bitmap reads colors in BGR format

#endif // DRAW_H
