#pragma once

#include "boundaryCondition.h"
#include <utility/cudaUtils.h>

struct BoundaryPatch {
	BoundaryConditionType type;

	CudaArray faceIDs;
	CudaArray values;
};