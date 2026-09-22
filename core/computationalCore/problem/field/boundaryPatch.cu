#include <problem/field/boundaryPatch.h>

ProblemBoundaryPatch::ProblemBoundaryPatch(
	const BoundaryConditionType& type,
	const std::vector<uint32_t>& faceIDs,
	const std::vector<double>& values,
	bool isDefault
	) :
	type(type), faceIDs(faceIDs), values(values), isDefault(isDefault)
{};