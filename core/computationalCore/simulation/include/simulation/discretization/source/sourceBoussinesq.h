#pragma once

#include <utility/cudaUtils.h>
#include <utility/utility.h>
#include <mesh/meshElements/meshElements.h>
#include <simulation/linearSolver/linearSolverMatrix.h>
#include <problem/field/field.h>

class SourceBoussinesq {

	using V = Vector<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;

public:

	__device__
		void assembleInner(
			const Mesh<MeshDim::D3>* mesh,
			LinearSolverMatrix<V>* matrix,
			V g,
			Field<double, C>* temperatureField,
			const double beta
		);
};