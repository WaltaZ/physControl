#pragma once

#include <vector>
#include <geometry/shapes/basic/surface.h>

enum class BoundaryConditionType {
	Drichlet,
	Neumann,
	Inlet,
	Outlet,
	Wall
};

struct BoundaryConditionD3 {

	BoundaryConditionType type;
	const std::vector<double> values;
	Surface<GeometryDim::D3> geometry;

	std::vector<uint32_t> faceIDs = {};

	BoundaryConditionD3(
		const BoundaryConditionType& type,
		const std::vector<double>& values,
		const Surface<GeometryDim::D3>& geometry
	);
};