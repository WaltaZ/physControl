#pragma once

#include "boundaryCondition.h"
#include <utility/cudaUtils.h>

struct ProblemBoundaryPatch {
	const BoundaryConditionType& type;

	const std::vector<uint32_t>& faceIDs;
	const std::vector<double>& values;

	ProblemBoundaryPatch(
		const BoundaryConditionType& type, 
		const std::vector<uint32_t>& faceIDs, 
		const std::vector<double>& values);
};

struct BoundaryPatch {
	BoundaryConditionType type;

	CudaArray<uint32_t> faceIDs;
	CudaArray<uint32_t> values;
};