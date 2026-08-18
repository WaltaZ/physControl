#pragma once

#include "basic/point.h"
#include "basic/surface.h"
#include "basic/volume.h"

struct Cuboid : public Volume {

	double a, b, c;

	Cuboid(
		const double& a,
		const double& b,
		const double& c,
		const Point<GeometryDim::D3>& startingPoint = Point<GeometryDim::D3>());
};