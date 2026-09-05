#pragma once

#include <utility/cudaUtils.h>
#include <utility/utility.h>
#include <mesh/meshElements/meshElements.h>
#include <simulation/linearSolver/linearSolverMatrix.h>

class SourceGravity {

	using V = Vector<GeometryDim::D3>;

public:

	__device__
		void assembleInner(
			CudaMesh<MeshDim::D3>* mesh,
			CudaLinearSolverMatrix<V>* matrix,
			V g
		);
};