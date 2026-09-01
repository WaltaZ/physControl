#include "../include/geometry/shapes/rectangle.h"

/*
RectangleD3::RectangleD3(
		double lengthA,
		double lengthB,
		std::array<double, 3> rotation = { 0, 0, 0 }) 
	: 
		lengthA(lengthA),
		lengthB(lengthB)
{
	// TODO: Implement
};*/

RectangleD3::RectangleD3( P* p1, P* p2, P* p3, P* p4) : Surface(std::vector<P*>({ p1, p2, p3, p4 })) {
	// TODO: Initilize a, b
}

RectangleD3::~RectangleD3()
{
	for (P* vertex : vertices) {
		delete vertex;
	}
}
