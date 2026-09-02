#pragma once

#include <problem/field.h>
#include <mesh/meshElements/meshElements.h>
#include <geometry/geometry.h>

template <MeshDim dim>
class Gradient {
public:
	virtual void compute(
		Field<double, Cell<dim>>& field,
		Field<Vector<mesh2geom(dim)>, Cell<dim>>& destField,
		Mesh<dim>& mesh
	) = 0;

	virtual void compute(
		Field<Vector<mesh2geom(dim)>, Cell<dim>>& field,
		Field<MatrixTensor<mesh2geom(dim)>, Cell<dim>>& destField,
		Mesh<dim>& mesh
	) = 0;
};