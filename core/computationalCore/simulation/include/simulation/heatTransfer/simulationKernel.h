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
}