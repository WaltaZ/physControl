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
			const Mesh<MeshDim::D3>* mesh,
			LinearSolverMatrix<V>* matrix,
			V g
		);
};