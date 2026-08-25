#include "include/problem/boundaryCondition.h"

BoundaryConditionD3::BoundaryConditionD3(
	const BoundaryConditionType& type,
	const std::vector<double>& values,
	const Surface<GeometryDim::D3>& geometry
) : type(type), values(values), geometry(geometry)
{};