#pragma once

#include <vector>
#include <geometry/shapes/basic/surface.h>

enum class BoundaryConditionType {
	Drichlet,
	Neumann,
	Mixed,
	Inlet,
	Outlet,
	NoSlip
};

struct BoundaryConditionD3Raw {

	BoundaryConditionType type;
	const std::vector<double> values;

	std::vector<uint32_t> faceIDs = {};

	BoundaryConditionD3Raw(
		const BoundaryConditionType& type,
		const std::vector<double>& values
	);
};

struct BoundaryConditionD3 : public BoundaryConditionD3Raw {

	Surface<GeometryDim::D3> geometry;

	BoundaryConditionD3(
		const BoundaryConditionType& type,
		const std::vector<double>& values,
		const Surface<GeometryDim::D3>& geometry
	);
};