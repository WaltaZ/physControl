#pragma once

#include <utility/cudaUtils.h>
#include <utility/debugUtils.h>

#include <simulation/simulationCore/simulationMethods.h>
#include <simulation/simulationCore/simulationConfig.h>
#include <simulation/simulationCore/simulationMethods.h>
#include <simulation/discretization/relaxation/relaxationPatankar.h>
#include <simulation/discretization/interpolation/interpolationTaylor.h>

#include <problem/problem.h>

namespace SIMPLEKernel {

	using V = Vector<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;
	using F = Face<MeshDim::D3>;
	using M = MatrixTensor<GeometryDim::D3>;

	__global__
		void assembleInnerVelocity(
			const Mesh<MeshDim::D3>* mesh,
			HeatTransferFieldsD3* fields,
			LinearSolverMatrix<V>* matrix,
			HeatTransferSimulationMethods* methods
		);

	__global__
		void assembleBoundariesVelocity(
			const Mesh<MeshDim::D3>* mesh,
			HeatTransferFieldsD3* fields,
			LinearSolverMatrix<V>* matrix
		);

	__global__
		void updateMassFlow(
			const Mesh<MeshDim::D3>* mesh,
			HeatTransferFieldsD3* fields,
			LinearSolverMatrix<V>* matrix
		);

	__global__
		void assemblePressure(
			const Mesh<MeshDim::D3>* mesh,
			HeatTransferFieldsD3* fields,
			LinearSolverMatrix<double>* p_matrix,
			LinearSolverMatrix<V>* V_matrix
		);

	__global__
		void correctPressureAndVelocity(
			const Mesh<MeshDim::D3>* mesh,
			HeatTransferFieldsD3* fields,
			Field<double, C>* pressureCorr,
			Field<V, C>* gradPressureCorr,
			LinearSolverMatrix<V>* V_matrix
		);

	__global__
		void correctMassFlow(
			const Mesh<MeshDim::D3>* mesh,
			HeatTransferFieldsD3* fields,
			Field<double, C>* pressureCorr,
			Field<V, C>* gradPressureCorr,
			LinearSolverMatrix<V>* V_matrix
		);

	template<class Obj>
	__global__
	void resetMatrix(
		LinearSolverMatrix<Obj>* matrix
	);
}