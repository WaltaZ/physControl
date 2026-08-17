#include "problemDependencies.h"

template<GeometryDim dim, typename Space>
struct ProblemGeometry {

	std::vector<Space> operationalSpace;

	ProblemGeometry(
		const std::vector<Space>& operationalSpace,
		const std::optional<std::vector<std::vector<int>>>& neighbouringBoundaries
	) : operationalSpace(operationalSpace)
	{};

	ProblemGeometry(
		const Space& operationalSpace
	) : operationalSpace(std::vector<Space>({ operationalSpace })) {};
};