#pragma once

#include "../meshEnums.h"

#include <array>
#include <geometry/point.h>

template <MeshDim dim>
struct Node {
	std::array<double, meshDimSize(dim)> pos;

	Node(const std::array<double, meshDimSize(dim)>& pos);
	Node(const Point<meshDimToGeometryDim(dim)>& point);
};