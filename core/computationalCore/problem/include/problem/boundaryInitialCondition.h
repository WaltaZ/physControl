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

template<typename FieldType>
struct BoundaryConditionD3 {
	FieldType** field;

	BoundaryConditionType type;
	const std::vector<double> values;

	Surface<GeometryDim::D3> geometry;
	std::vector<uint32_t> faceIDs = {};
};

template<typename VariableType, typename FieldType>
struct InitialCondition {
	FieldType** field;
	VariableType value;
};