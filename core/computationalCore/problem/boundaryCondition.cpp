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