#pragma once

#include "linearSolver.h"

template<class Obj>
class LinearSolverJacobi : public LinearSolver<Obj> {
public:

	using LinearSolver<Obj>::matrix;
	using LinearSolver<Obj>::_field;
	using LinearSolver<Obj>::_mesh;

	LinearSolverJacobi(
		Mesh<MeshDim::D3>* mesh,
		Field<Obj, Cell<MeshDim::D3>>* field
	);

	void solve() override;
};