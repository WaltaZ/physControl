#pragma once

#include "problemGeometry.h"
#include "field.h"
#include "boundaryCondition.h"

#include <geometry/geometry.h>
#include <mesh/meshElements/meshElements.h>
#include <mesh/meshers/mesherElements.h>
#include <vector>

class ProblemD3 {
public:
	ProblemGeometryD3 geometry;
	std::vector<std::vector<BoundaryConditionD3>> boundaryConditions;
	// TODO: Define this \/
	std::vector<BoundaryConditionD3Raw> defaultBoundaryConditions;

	ProblemD3(const ProblemGeometryD3& geometry);

	virtual void initBoundaryPatches(
		const std::vector<MesherBoundaryCondition>& mesherBC,
		const std::vector<MesherBoundaryConditionRaw>& mesherBCDefault
	) = 0;

	virtual void initFields(const Mesh<MeshDim::D3>& mesh) = 0;
};

class HeatTransferFieldsD3 {
private:
	using V = Vector<GeometryDim::D3>;
	using M = MatrixTensor<GeometryDim::D3>;
	using C = Cell<geometryDimToMeshDim(GeometryDim::D3)>;
	using F = Face<geometryDimToMeshDim(GeometryDim::D3)>;
public:
	MainField<V, C> velocity = MainField<V, C>(V({0, 0, 0}));
	MainField<double, C> temperature = MainField<double, C>(20);
	Field<double, F> massFlowRate = Field<double, F>(0);
	Field<M, C> gradVelocity = Field<M, C>(M({
		0, 0, 0,
		0, 0, 0, 
		0, 0, 0
	}));
	Field<double, C> pressure = Field<double, C>(10e5);
	Field<V, C> gradPressure = Field<V, C>(V({ 0, 0, 0 }));
};

class HeatTransferD3 : public ProblemD3 {
public:
	HeatTransferFieldsD3 fields{};

	HeatTransferD3(
		const ProblemGeometryD3& geometry
	);

	void initBoundaryPatches(
		const std::vector<MesherBoundaryCondition>& mesherBC,
		const std::vector<MesherBoundaryConditionRaw>& mesherBCDefault) override;

	void initFields(const Mesh<MeshDim::D3>& mesh) override;

	void addVelocityBoundaryCondition(
		const BoundaryConditionD3& bc);

	void addTemperatureBoundaryCondition(
		const BoundaryConditionD3& bc);

	void addVelocityInitialCondition(const Vector<GeometryDim::D3>& ic);

	void addTemperatureInitialCondition(const double& ic);
};