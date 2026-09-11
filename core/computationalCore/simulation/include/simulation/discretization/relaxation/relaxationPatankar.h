#pragma once

#include <utility/cudaUtils.h>

#include <simulation/linearSolver/linearSolverMatrix.h>
#include <problem/field/field.h>

class RelaxationPatankar {
	template<class Obj>
	__device__
		void relax(
			LinearSolverMatrix<Obj>* matrix,
			Field<Obj, Cell<MeshDim::D3>>* field,
			double relaxCoeff
		);
};