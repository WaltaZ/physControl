#pragma once

#include <problem/field.h>
#include <mesh/meshElements/meshElements.h>
#include <geometry/geometry.h>

class GradientBase {
public:
	virtual void compute(
		Field<double, Cell<MeshDim::D3>>& field,
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
		Mesh<MeshDim::D3>& mesh
	) = 0;

	virtual void compute(
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& field,
		Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
		Mesh<MeshDim::D3>& mesh
	) = 0;
};

template<class Derived>
class Gradient : public GradientBase {
public:
	void compute(
		Field<double, Cell<MeshDim::D3>>& field,
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
		Mesh<MeshDim::D3>& mesh
	) override 
	{
		static_cast<Derived*>(this)->
			template computeImpl<double, Vector<GeometryDim::D3>>(
				field,
				destField,
				mesh
			);
	};

	void compute(
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& field,
		Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
		Mesh<MeshDim::D3>& mesh
	) override 
	{
		static_cast<Derived*>(this)->
			template computeImpl<Vector<GeometryDim::D3>, MatrixTensor<GeometryDim::D3>>(
				field,
				destField,
				mesh
			);
	};
};