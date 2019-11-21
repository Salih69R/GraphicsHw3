



//assumes x1 < x2, y1 < y2, dy = y2-y1 < dx = x2-x1
void basicMidPointDraw(int x1, int y1, int x2, int y2) {

	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, E, NE;
	x = x1;
	y = y1;
	d = 2 * dy - dx;
	E = 2 * dy;
	NE = 2 * (dy - dx);

	//plotPixel(x,y)

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
		//plotPixel(x,y)
	}
}



void MidPointDraw(int x1, int y1, int x2, int y2) {
	
	if (x2 < x1) // make sure that x1 <= x2, the == situation is ok since it just wont draw anything
	{
		MidPointDraw(x2, y2, x1, y1);
		return;
	}
	if (y2 < y1) {//the == situation is ok since it just wont draw anything
		//mirror around X, now y2 >= y1
		
		if (y2 - y1 >= x2 - x1) {
			//miror around X=Y, this makes dy<dx
			basicMidPointDraw(x1, y1, x2, y2);
			//mirro back around X=Y
		}
		else {
			basicMidPointDraw(x1, y1, x2, y2);
		}	
		// then re mirror back around X
	}

}




