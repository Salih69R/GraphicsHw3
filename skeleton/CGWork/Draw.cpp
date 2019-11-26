
#include "Draw.h"


//takes x1 y1 that are already set to the needed color, copys the color of the pixel to x y
void cpyPixel(CDC* dcdest, CDC* dcsrc, int x, int y, int x1, int y1) {
	dcdest->BitBlt(x, y, 1, 1, dcsrc, x1, y1, SRCCOPY);
}
//m_pDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pDCToUse, r.left, r.top, SRCCOPY);

void setPix(CDC* dc, int x, int y, COLORREF color) {


	CBitmap MyBmp;


	MyBmp.CreateCompatibleBitmap(dc, 1, 1);

	dc->SelectObject(&MyBmp);


	MyBmp.SetBitmapBits(1, &color);//not sure this sets it the right way

	//still need to actaully draw it in x y





}



//assumes x1 < x2, y1 < y2,  0 < dy <= dx 
void basicMidPointDraw1(int x1, int y1, int x2, int y2, CDC* hdc, COLORREF color) {

	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, E, NE;
	x = x1;
	y = y1;
	d = 2 * dy - dx;
	E = 2 * dy;
	NE = 2 * (dy - dx);

	SetPixel(*hdc, x, y, color);



	while (x < x2) {


		if (d < 0) {
			d += E;
			++x;
		}
		else {
			d += NE;
			++x;
			++y;
		}
		cpyPixel(hdc, hdc, x, y, x1, y1);
	}
}

//assumes x1 < x2, y1 < y2,           dy  > dx  >= 0
void basicMidPointDraw2(int x1, int y1, int x2, int y2, CDC* hdc, COLORREF color) {

	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, N, NE;
	x = x1;
	y = y1;
	d = 2 * dx - dy;
	N = 2 * dx;
	NE = 2 * (dx - dy);

	SetPixel(*hdc, x, y, color);

	while (y < y2) {


		if (d < 0) {
			d += N;
			++y;
		}
		else {
			d += NE;
			++x;
			++y;
		}
		cpyPixel(hdc, hdc, x, y, x1, y1);
	}
}




//assumes x1 < x2, y1 > y2,       -dx < dy < 0 < dx 
void basicMidPointDraw3(int x1, int y1, int x2, int y2, CDC* hdc, COLORREF color) {

	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, E, SE;
	x = x1;
	y = y1;


	d = 2 * dy - dx;//-200
	E = 2 * dy;//-100 this should be positive i guess
	SE = 2 * (dy + dx);//100 and this should be negative

	SetPixel(*hdc, x, y, color);

	while (x < x2) {


		if (d > 0) {//so i've just used the complimntary condition
			d += E;
			++x;
		}
		else {
			d += SE;
			++x;
			--y;
		}
		cpyPixel(hdc, hdc, x, y, x1, y1);
	}
}




//assumes x1 < x2, y1 > y2,           dy < -dx < 0 < dx < -dy
void basicMidPointDraw4(int x1, int y1, int x2, int y2, CDC* hdc, COLORREF color) {

	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, S, SE;

	x = x1;
	y = y1;


	d = 2 * dy - dx;
	S = 2 * dx;
	SE = 2 * (dy + dx);

	SetPixel(*hdc, x, y, color);

	while (y > y2) {


		if (d < 0) {
			d += S;
			--y;
		}
		else {
			d += SE;
			++x;
			--y;
		}
		cpyPixel(hdc, hdc, x, y, x1, y1);

	}
}




void MidPointDraw(int x1, int y1, int x2, int y2, CDC* hdc, COLORREF color) {

	if (x2 < x1) // make sure that x1 <= x2, the == situation is ok
	{
		MidPointDraw(x2, y2, x1, y1, hdc, color);
		return;
	}
	//now  x1 <= x2

	if (y1 <= y2) {
		if (y2 - y1 <= x2 - x1)
			basicMidPointDraw1(x1, y1, x2, y2, hdc, color);
		else
			basicMidPointDraw2(x1, y1, x2, y2, hdc, color);
		return;
	}

	//now  x1 <= x2 && y1 > y2

	if (x1 - x2 < y2 - y1)
		basicMidPointDraw3(x1, y1, x2, y2, hdc, color); //-dx < dy < 0 < dx
	else
		basicMidPointDraw4(x1, y1, x2, y2, hdc, color); //dy < -dx < 0 < dx < -dy
	return;
}




