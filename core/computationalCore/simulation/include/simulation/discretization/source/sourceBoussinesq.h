#pragma once

#include <utility/cudaUtils.h>
#include <utility/utility.h>
#include <mesh/meshElements/meshElements.h>
#include <simulation/linearSolver/linearSolverMatrix.h>
#include <problem/field.h>

class SourceBoussinesq {

	using V = Vector<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;

public:

	__device__
		void assembleInner(
			CudaMesh<MeshDim::D3>* mesh,
			CudaLinearSolverMatrix<V>* matrix,
			V g,
			CudaField<double, C>* temperatureField,
			const double T_0,
			const double beta
		);
};