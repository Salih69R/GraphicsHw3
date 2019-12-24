#ifndef DRAW_H
#define DRAW_H


#include <afxwin.h>
#include "Poly.h"


Vec2u coordsToPixels(const double &x, const double &y, const uint &width, const uint &height);

void ScaneConvert(Poly & poly, Tmatd& transf, int* bits, int width, int height, COLORREF color, double NEAR_PLANE);


void MidPointDraw(int x1, int y1, int x2, int y2, int* bits, COLORREF color, int width, int height);
COLORREF RGBToBGR(COLORREF col);//used to switch the order because for some reason the bitmap reads colors in BGR format

#endif // DRAW_H
