#pragma once

#include <vector>
#include <geometry/shapes/basic/surface.h>

enum class BoundaryConditionType {
	Drichlet = 1,
	Neumann = 2,
	Mixed = 3,
	Inlet = 4,
	Outlet = 5,
	NoSlip = 6
};

std::string getBoundaryConditionType(BoundaryConditionType type);

BoundaryConditionType getBoundaryConditionType(const std::string& type);

struct BoundaryConditionD3Raw {

	BoundaryConditionType type;
	const std::vector<double> values;

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