#pragma once

#include "field.h"

enum class BoundaryConditionType {
	Drichlet,
	Neumann,
	Inlet,
	Outlet,
	Wall
};

template<typename FieldType>
struct BoundaryCondition {
	FieldType* field;
};