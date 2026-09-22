#pragma once

#include "problemGeometry.h"
#include "field/field.h"
#include "boundaryCondition.h"

#include <geometry/geometry.h>
#include <mesh/meshElements/meshElements.h>
#include <mesh/meshers/mesherElements.h>
#include <utility/cudaUtils.h>
#include <vector>

#include <fileStream/path.h>

class ProblemD3 {
public:
	Volume geometry;
	std::vector<std::vector<BoundaryConditionD3>> boundaryConditions;
	// TODO: Define this \/
	std::vector<BoundaryConditionD3Raw> defaultBoundaryConditions;

	ProblemD3(const Volume& geometry);

	ProblemD3() = default;

	virtual void initBoundaryPatches(
		const std::vector<MesherBoundaryCondition>& mesherBC,
		const std::vector<MesherBoundaryConditionRaw>& mesherBCDefault
	) = 0;

	virtual void initFields(const Mesh<MeshDim::D3>* mesh) = 0;

	virtual void save(
		const std::string& path = 
		path::parse({ path::CACHE_DIR, "boundaryConditions" })) = 0;

	virtual void load(
		const std::string& path = 
		path::parse({ path::CACHE_DIR, "boundaryConditions" })) = 0;
};

class HeatTransferFieldsD3 {
private:
	using V = Vector<GeometryDim::D3>;
	using M = MatrixTensor<GeometryDim::D3>;
	using C = Cell<geom2mesh(GeometryDim::D3)>;
	using F = Face<geom2mesh(GeometryDim::D3)>;
public:
	Field<V, C>* velocity = cudaUtils::create<Field<V, C>>(
		std::string("velocity"),
		V({0, 0, 0})
	);
	Field<M, C>* gradVelocity = cudaUtils::create<Field<M, C>>(
		std::string("gradVelocity"),
		M({
			0, 0, 0,
			0, 0, 0, 
			0, 0, 0
		}));
	Field<double, C>* temperature = cudaUtils::create<Field<double, C>>(
		std::string("temperature"),
		298
	);
	Field<V, C>* gradTemperature = cudaUtils::create<Field<V, C>>(
		std::string("gradTemperature"),
		V({ 0, 0, 0 })
	);
	Field<double, F>* massFlowRate = cudaUtils::create<Field<double, F>>(
		std::string("massFlowRate"),
		0.0
	);
	Field<double, C>* pressure = cudaUtils::create<Field<double, C>>(
		std::string("pressure"),
		0.8305e5
	);
	Field<V, C>* gradPressure = cudaUtils::create<Field<V, C>>(
		std::string("gradPressure"),
		V({ 0, 0, 0 })
	);
};

class HeatTransferProblemD3 : public ProblemD3 {
public:
	HeatTransferFieldsD3* fields;

	HeatTransferProblemD3(
		const Volume& geometry
	);

	HeatTransferProblemD3();

	void initBoundaryPatches(
		const std::vector<MesherBoundaryCondition>& mesherBC,
		const std::vector<MesherBoundaryConditionRaw>& mesherBCDefault) override;

	void initFields(const Mesh<MeshDim::D3>* mesh) override;

	void addVelocityBoundaryCondition(
		const BoundaryConditionD3& bc);

	void addTemperatureBoundaryCondition(
		const BoundaryConditionD3& bc);

	void addVelocityInitialCondition(const Vector<GeometryDim::D3>& ic);

	void addTemperatureInitialCondition(const double& ic);

	void save(
		const std::string& path = 
		path::parse({ path::CACHE_DIR, "problem" })) override;

	void load(
		const std::string& path = 
		path::parse({ path::CACHE_DIR, "problem" })) override;

private:
	void _initDefault();
};