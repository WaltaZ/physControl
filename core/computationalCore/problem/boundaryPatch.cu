#include "include/problem/boundaryPatch.h"

ProblemBoundaryPatch::ProblemBoundaryPatch(
	const BoundaryConditionType& type,
	const std::vector<uint32_t>& faceIDs,
	const std::vector<double>& values) :
	type(type), faceIDs(faceIDs), values(values)
{};