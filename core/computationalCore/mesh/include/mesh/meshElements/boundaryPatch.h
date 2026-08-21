#pragma once

#include <problem/boundaryInitialCondition.h>

class BoundaryPatch {
	BoundaryConditionType type;

	int* faceIDs;
	int faceIDsLength;

	double* values;
	int valuesLength;
};