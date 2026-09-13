#pragma once

#include <utility/cudaUtils.h>

#include <simulation/heatTransfer/simulationMethods.h>

namespace simKernel {

	using V = Vector<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;
	using F = Face<MeshDim::D3>;
	using M = MatrixTensor<GeometryDim::D3>;
	
	__global__
	void assembleInnerVelocity(
		const Mesh<MeshDim::D3>* mesh,
		Field<V, C>* velocity,
		Field<M, C>* gradVelocity,
		Field<double, C>* temperature,
		Field<double, F>* massFlowRate,
		Field<V, C>* gradPressure,
		LinearSolverMatrix<V>* matrix,
		HeatTransferSimulationMethods* methods
	);

	__global__
	void assembleBoundariesVelocity(
		const Mesh<MeshDim::D3>* mesh,
		Field<V, C>* velocity,
		Field<double, C>* pressure,
		Field<V, C>* gradPressure,
		LinearSolverMatrix<V>* matrix
	);

	__global__
	void updateMassFlow(
		const Mesh<MeshDim::D3>* mesh,
		Field<V, C>* velocity,
		Field<double, C>* pressure,
		Field<V, C>* gradPressure,
		Field<double, F>* massFlowRate,
		LinearSolverMatrix<V>* matrix
	);

	__global__
	void assemblePressure(
		const Mesh<MeshDim::D3>* mesh,
		Field<double, F>* massFlowRate,
		LinearSolverMatrix<double>* matrix
	);
}