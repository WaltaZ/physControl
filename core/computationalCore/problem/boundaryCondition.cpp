#include "include/problem/boundaryCondition.h"

BoundaryConditionD3Raw::BoundaryConditionD3Raw(
	const BoundaryConditionType& type,
	const std::vector<double>& values
) : type(type), values(values) {};

BoundaryConditionD3::BoundaryConditionD3(
	const BoundaryConditionType& type,
	const std::vector<double>& values,
	const Surface<GeometryDim::D3>& geometry
) : BoundaryConditionD3Raw(type, values), geometry(geometry)
{};

std::string getBoundaryConditionType(BoundaryConditionType type) 
{
	switch (type) {
	case BoundaryConditionType::Drichlet:
		return "drichlet";

	case BoundaryConditionType::Neumann:
		return "neumann";

	case BoundaryConditionType::Mixed:
		return "mixed";

	case BoundaryConditionType::Inlet:
		return "inlet";

	case BoundaryConditionType::Outlet:
		return "outlet";

	case BoundaryConditionType::NoSlip:
		return "noSlip";
	};
};

BoundaryConditionType getBoundaryConditionType(const std::string& type) {
	if (type == "drichlet") {
		return BoundaryConditionType::Drichlet;
	}
	else if (type == "neumann") {
		return BoundaryConditionType::Neumann;
	}
	else if (type == "mixed") {
		return BoundaryConditionType::Mixed;
	}
	else if (type == "inlet") {
		return BoundaryConditionType::Inlet;
	}
	else if (type == "outlet") {
		return BoundaryConditionType::Outlet;
	}
	else if (type == "noSlip") {
		return BoundaryConditionType::NoSlip;
	}
};