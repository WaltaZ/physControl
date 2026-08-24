#pragma once

#include <problem/boundaryInitialCondition.h>
#include <utility/cudaUtils.h>

class BoundaryPatch {
	BoundaryConditionType type;

	CudaArray faceIDs;
	CudaArray values;
};