#pragma once

#include <problem/field.h>
#include <mesh/meshElements/meshElements.h>
#include <utility/cudaUtils.h>

#include "linearSolverMatrix.h"

template<class Obj>
class LinearSolver {
protected:
	const Mesh<MeshDim::D3>* _mesh;
	Field<Obj, Cell<MeshDim::D3>>* _field;
public:
	LinearSolverMatrix<Obj>* matrix;
	
	virtual void solve() = 0;

	LinearSolver(
		const Mesh<MeshDim::D3>* mesh,
		Field<Obj, Cell<MeshDim::D3>>* field
	) : _mesh(mesh), _field(field) 
	{
		matrix = cudaUtils::create<LinearSolverMatrix<Obj>>(mesh);
	};
};