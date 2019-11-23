#ifndef GEOMETRY
#define GEOMETRY


#include <vector>
#include <Windows.h>
using std::vector;

class Geometry
{
public:

	vector<double[3]> vertixes;
	COLORREF color;

	Geometry();
	~Geometry();
};

#endif