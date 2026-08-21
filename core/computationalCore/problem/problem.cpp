#include "include/problem/problem.h"

	RoomHeatTransferD3::RoomHeatTransferD3(
		const ProblemGeometryD3& geometry)
		:
		geometry(geometry) {
	};

	void RoomHeatTransferD3::addVelocityBoundaryCondition(
		const BoundaryConditionD3<Field<V, C>>& bc)
	{
		velocityBoundaries.push_back(bc);
	};

	void RoomHeatTransferD3::addTemperatureBoundaryCondition(
		const BoundaryConditionD3<Field<double, C>>& bc)
	{
		temperatureBoundaries.push_back(bc);
	};