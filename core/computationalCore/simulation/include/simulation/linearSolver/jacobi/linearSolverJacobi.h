#pragma once

#include "../linearSolver.h"

template<class Obj>
class LinearSolverJacobi : public LinearSolver<Obj> {
public:

	using LinearSolver<Obj>::matrix;
	using LinearSolver<Obj>::_field;
	using LinearSolver<Obj>::_mesh;

	LinearSolverJacobi(
		const Mesh<MeshDim::D3>* mesh,
		Field<Obj, Cell<MeshDim::D3>>* field
	);

	void solve() override;
};

class LinearSolverJacobiFactory :
	public LinearSolverFactory {

	std::unique_ptr<LinearSolver<double>> getSolver(
		const Mesh<MeshDim::D3>* mesh,
		Field<double, Cell<MeshDim::D3>>* field
	) override;

	std::unique_ptr<LinearSolver<Vector<GeometryDim::D3>>> getSolver(
		const Mesh<MeshDim::D3>* mesh,
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* field
	) override;
};