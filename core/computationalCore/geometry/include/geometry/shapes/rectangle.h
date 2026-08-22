#pragma once

#include "../geometryEnums.h"
#include "basic/surface.h"

struct RectangleD3 : Surface<GeometryDim::D3> {

	using P = Point<GeometryDim::D3>;

	double lengthB, lengthBb;

	/*RectangleD3(
		double lengthA,
		double lengthB,
		std::array<double, 3> rotation = { 0, 0, 0 }
	);*/

	RectangleD3(P* p1, P* p2, P* p3, P* p4);

	~RectangleD3();
};