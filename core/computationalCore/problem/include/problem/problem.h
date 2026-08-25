#pragma once

#include "problemGeometry.h"
#include "field.h"
#include "boundaryInitialCondition.h"

#include <geometry/geometry.h>
#include <mesh/meshElements/meshElements.h>
#include <vector>


class RoomHeatTransferD3 {
private:
	using V = Vector<GeometryDim::D3>;
	using M = MatrixTensor<GeometryDim::D3>;
	using C = Cell<geometryDimToMeshDim(GeometryDim::D3)>;
	using F = Face<geometryDimToMeshDim(GeometryDim::D3)>;

public:
	Field<V, C>* velocity;
	Field<M, C>* gradVelocity;
	Field<double,C>* pressure;
	Field<V, C>* gradPressure;
	Field<double, C>* temperature;

	ProblemGeometryD3 geometry;

	std::vector<std::vector<BoundaryConditionD3>> boundaryConditions{2};
	// TODO: Define this \/
	std::vector<BoundaryConditionD3> defaultBoundaryConditions{ };
	
	RoomHeatTransferD3(
		const ProblemGeometryD3& geometry
	);

	void addVelocityBoundaryCondition(
		const BoundaryConditionD3& bc);

	void addTemperatureBoundaryCondition(
		const BoundaryConditionD3& bc);
};