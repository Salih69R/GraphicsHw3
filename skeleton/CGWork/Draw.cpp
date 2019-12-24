
#include "Draw.h"
#include <functional>
#include <array>
#include <set>





Vec2u coordsToPixels(const double &x, const double &y, const uint &width, const uint &height)
{
	double width_d = static_cast<double>(width);
	double height_d = static_cast<double>(height);

	uint x_res = static_cast<uint>((width_d / 2.0) * (x + 1.0));
	uint y_res = static_cast<uint>((height_d / 2.0) * (1.0 - y));


	return Vec2u(x_res, y_res);
}





bool isOutOfBount(int x, int y, int w, int h) {

	return x < 0 || y < 0 || x >= w || y >= h;
}
//assumes x1 < x2, y1 < y2,  0 < dy <= dx , assume x1 and y1 are inBound
void basicMidPointDraw1(int x1, int y1, int x2, int y2, int* bits, COLORREF color, int w, int h) {

	int firstX = -1, firstY = -1;
	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, E, NE;
	x = x1;
	y = y1;
	d = 2 * dy - dx;
	E = 2 * dy;
	NE = 2 * (dy - dx);


	if (!isOutOfBount(x, y, w, h)) {
		bits[x + w *y] = color;
		//SetPixel(*hdc, x, y, color);
		firstX = x;
		firstY = y;
	}

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
		if (!isOutOfBount(x, y, w, h)) {
			if (firstX > -1 && firstY > -1)
				bits[x + w * y] = color;
				//cpyPixel(hdc, hdc, x, y, firstX, firstY);
			else
			{
				bits[x + w * y] = color;
				//SetPixel(*hdc, x, y, color);
				firstX = x;
				firstY = y;
			}
		}
	}
}

//assumes x1 < x2, y1 < y2,           dy  > dx  >= 0 , assume x1 and y1 are inBound
void basicMidPointDraw2(int x1, int y1, int x2, int y2, int* bits, COLORREF color, int w, int h) {

	int firstX = -1, firstY = -1;
	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, N, NE;
	x = x1;
	y = y1;
	d = 2 * dx - dy;
	N = 2 * dx;
	NE = 2 * (dx - dy);


	if (!isOutOfBount(x, y, w, h)) {
		bits[x + w * y] = color;
		//SetPixel(*hdc, x, y, color);
		firstX = x;
		firstY = y;
	}


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
		if (!isOutOfBount(x, y, w, h)) {
			if (firstX > -1 && firstY > -1)
				bits[x + w * y] = color;
				//cpyPixel(hdc, hdc, x, y, firstX, firstY);
			else
			{
				bits[x + w * y] = color;
				//SetPixel(*hdc, x, y, color);
				firstX = x;
				firstY = y;
			}
		}
	}


}




//assumes x1 < x2, y1 > y2,       -dx < dy < 0 < dx  , assume x1 and y1 are inBound
void basicMidPointDraw3(int x1, int y1, int x2, int y2, int* bits, COLORREF color, int w, int h) {

	int firstX = -1, firstY = -1;
	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, E, SE;
	x = x1;
	y = y1;


	d = 2 * dy - dx;//-200
	E = 2 * dy;//-100 this should be positive i guess
	SE = 2 * (dy + dx);//100 and this should be negative


	if (!isOutOfBount(x, y, w, h)) {
		bits[x + w * y] = color;
		//SetPixel(*hdc, x, y, color);
		firstX = x;
		firstY = y;
	}



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
		if (!isOutOfBount(x, y, w, h)) {
			if (firstX > -1 && firstY > -1)
				bits[x + w * y] = color;
				//cpyPixel(hdc, hdc, x, y, firstX, firstY);
			else
			{
				bits[x + w * y] = color;
				//SetPixel(*hdc, x, y, color);
				firstX = x;
				firstY = y;
			}
		}
	}


}




//assumes x1 < x2, y1 > y2,           dy < -dx < 0 < dx < -dy , assume x1 and y1 are inBound
void basicMidPointDraw4(int x1, int y1, int x2, int y2, int* bits, COLORREF color, int w, int h) {

	int firstX = -1, firstY = -1;
	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, S, SE;

	x = x1;
	y = y1;


	d = 2 * dy - dx;
	S = 2 * dx;
	SE = 2 * (dy + dx);



	if (!isOutOfBount(x, y, w, h)) {
		bits[x + w * y] = color;
		//SetPixel(*hdc, x, y, color);
		firstX = x;
		firstY = y;
	}


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
		if (!isOutOfBount(x, y, w, h)) {
			if (firstX > -1 && firstY > -1)
				bits[x + w * y] = color;
				//cpyPixel(hdc, hdc, x, y, firstX, firstY);
			else
			{
				bits[x + w * y] = color; 
				//SetPixel(*hdc, x, y, color);
				firstX = x;
				firstY = y;
			}
		}

	}

}



//color must be in BGR format not RGB
void MidPointDraw(int x1, int y1, int x2, int y2, int* bits, COLORREF color, int width, int height) {

	if (x2 < x1) // make sure that x1 <= x2, the == situation is ok
	{
		MidPointDraw(x2, y2, x1, y1, bits, color, width, height);
		return;
	}
	//now  x1 <= x2
	if ((x1 < 0 && x2 < 0) || (y1 < 0 && y2 < 0) || (x1 > width && x2 >width) || (y1 > height && y2 >height) )
		return;


	//take care of cases the are going out of screen bound
	double dx = x2 - x1, dy = y2 - y1;
	if (x1 < 0)
	{
		if(dx!=0)
			MidPointDraw(0, y2 - (x2 * (dy / dx)), x2, y2, bits, color, width, height);
		return;
	}
	if (y1 < 0) {

		if (dy != 0)
			MidPointDraw(x2 - (y2*(dx / dy)), 0, x2, y2, bits, color,  width,  height);
		return;
	}

	if (y1 > height - 1) {

		if (dy != 0)
			MidPointDraw( x2 + (height - y2 - 1) * (dx / dy) , height -1, x2, y2, bits, color, width, height);
		return;
	}
	
	


	if (y1 <= y2) {
		if (y2 - y1 <= x2 - x1)
			basicMidPointDraw1(x1, y1, x2, y2, bits, color, width , height);
		else
			basicMidPointDraw2(x1, y1, x2, y2, bits, color, width, height);
		return;
	}

	//now  x1 <= x2 && y1 > y2

	if (x1 - x2 < y2 - y1)
		basicMidPointDraw3(x1, y1, x2, y2, bits, color, width, height); //-dx < dy < 0 < dx
	else
		basicMidPointDraw4(x1, y1, x2, y2, bits, color, width, height); //dy < -dx < 0 < dx < -dy
	return;
}

COLORREF RGBToBGR(COLORREF col) {
	return RGB(GetBValue(col), GetGValue(col), GetRValue(col));
}


















//***********************code for scane conversion*****************************************




struct Line {

	Vec2u p1, p2;
	int minY, minX, maxY, maxX;
	/*
		line equation params ( y = mx +b )
		if (isMZero && isDxInfinity) the line is just one point
		if (!isDxInfinity) this means isMZero = true, so the equation is y = constant
		this means isDxInfinityy = true && isMZero == true,, so thge equation is x = constant, for this situation dont use the b and m
	*/
	int m, b;
	bool isDxInfinity, isMZero;

	Line(Vec2u p1, Vec2u p2) : p1(p1), p2(p2)  {
		minY = p1(1) < p2(1) ? p1(1) : p2(1);
		maxY = p1(1) > p2(1) ? p1(1) : p2(1);
		minX = p1(0) < p2(0) ? p1(0) : p2(0);
		maxX = p1(0) > p2(0) ? p1(0) : p2(0);


		isDxInfinity = isMZero = false;
		if (p2(0) - p1(0) == 0) 
			isDxInfinity = true;
		if (p2(1) - p1(1) == 0) 
			isMZero = true;
		
			
		if(!isDxInfinity && !isMZero)
		{
			m = (p2(1) - p1(1)) / (p2(0) - p1(0));//=dy/dx
			b = p1(1) - m * p1(0);//=y1-m*x1
			
		}
		else {
			m = 0;
			b = p1(1);
		}
		
	}

	Line(const Line& line) = default;
	
	Vec2u intersectWithY(int y) const {

		if (isMZero || m == 0) //equation is y = constant(b), and the y line doesn't touch this line (we ignor the situation where this line is a pointif b == y also dont draw this since we ignore coloring the wireframs
			throw (-1);

		if (isDxInfinity) {
			return Vec2u(p1(0), y);
		}


		//find the x of intersection
		int x = (y - b) / m;
		if (x < minX || x > maxX)
			throw (-1);
		return Vec2u(x, y);
	}

	//compare by minY
	friend bool operator<(const Line& a, const Line& b) {
		return a.minY < b.minY;
	}

	friend bool operator==(const Line& a,const Line& b) {

		return ( a.minX == b.minX && a.minY == b.minY && a.maxX==b.maxX && a.maxY == b.maxY);
	}
};





void ScaneConvert(Poly & poly, Tmatd& transf,int* bits,  int width, int height, COLORREF color, double NEAR_PLANE)
{
	
	auto vers = poly.getVertices();
	std::vector<Line> sortedlines;
	Vec2u first_vertex_px;

	for (unsigned i = 0; i < vers.size() - 1; i++)
	{
		Vec4d p1 = transf * vers[i];
		Vec4d p2 = transf * vers[i + 1];

		if (p1(3) < NEAR_PLANE) p1(3) = NEAR_PLANE;
		if (p2(3) < NEAR_PLANE) p2(3) = NEAR_PLANE;

		p1 /= p1(3);
		p2 /= p2(3);

		auto px1 = coordsToPixels(p1(0), p1(1), width, height);
		auto px2 = coordsToPixels(p2(0), p2(1), width, height);

		if (i == 0)
		{
			first_vertex_px = px1;
		}

		Line line = Line(px1,px2);
		sortedlines.push_back(line);

		if (i == vers.size() - 2)
		{
			Line line = Line(px2, first_vertex_px);
			sortedlines.push_back(line);
		}
	}

	

	//sort by Ymin (accending order)
	std::sort(sortedlines.begin(), sortedlines.end(),
            [](const Line& a, const Line& b)
            {
                return a.minY < b.minY;
            });


	//scan convert alg
	std::vector<Line> relevantLines = std::vector<Line>();
	for (int y = 0; y < height; ++y)
	{
		//add all lines with minY <= y that haven't been checked yet (maxY > y)
		for (size_t i = 0; i < sortedlines.size(); i++)
		{
			if (sortedlines[i].minY >= y)
				break;
			auto& it = std::find(relevantLines.begin(), relevantLines.end(), sortedlines[i]);
			
			if (sortedlines[i].maxY < y && it != relevantLines.end())//remove all lines with maxY < y
				relevantLines.erase(it);
			else if (sortedlines[i].maxY >= y && it == relevantLines.end())
				relevantLines.push_back(sortedlines[i]);
		}
		 
		//get all points of intersection for this y with all the relevant lines
		std::vector<Vec2u> Intersections = std::vector<Vec2u>();
		for (auto &curLine : relevantLines) {

			//no need to fill a horizental line that will be drawn as a wirefram
			try {
				Intersections.push_back(curLine.intersectWithY(y));
			}
			catch (int a) {
				//do nothing, just don't draw it and chill
				continue;
			}
		}
		//sort in ascneding X order
		std::sort(Intersections.begin(), Intersections.end(),
			[](const Vec2u& a, const Vec2u& b)
		{
			return a(0) < b(0);
		});

		//TODO: understand and fix the problem of deciding wether to draw the first line or not in the scan conversion (from 0 to point1 or from p1 to p2)
		
		for (int i = 0; i < (int) Intersections.size() - 1; i += 2) {
			//actually drawing the lines using midpoint
			MidPointDraw(Intersections[i](0), Intersections[i](1), Intersections[i + 1](0), Intersections[i + 1](1), bits, RGBToBGR(color), width, height);
		}
		
	}
	
}











