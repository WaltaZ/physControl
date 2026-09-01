#pragma once

#include "../point.h"
#include "basic/surface.h"
#include "basic/volume.h"

struct Cuboid : public Volume {

	enum class FaceType {
		Front,
		Back,
		Bottom,
		Top,
		Left,
		Right
	};

	const static FaceType faceOrder[6];

	double a, b, c;

	Cuboid(
		const double& a,
		const double& b,
		const double& c,
		const Point<GeometryDim::D3>& startingPoint = Point<GeometryDim::D3>());
};