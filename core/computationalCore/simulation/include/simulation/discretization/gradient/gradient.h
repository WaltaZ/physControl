#pragma once

#include <problem/field.h>
#include <mesh/meshElements/meshElements.h>
#include <geometry/geometry.h>

class GradientBase {
public:
	virtual void compute(
		const Field<double, Cell<MeshDim::D3>>* field,
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* gradField,
		const Mesh<MeshDim::D3>* mesh
	) = 0;

	virtual void compute(
		const Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* field,
		Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>* gradField,
		const Mesh<MeshDim::D3>* mesh
	) = 0;
};

template<class Derived>
class Gradient : public GradientBase {
public:
	void compute(
		const Field<double, Cell<MeshDim::D3>>* field,
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* gradField,
		const Mesh<MeshDim::D3>* mesh
	) override 
	{
		static_cast<Derived*>(this)->
			template computeImpl<double, Vector<GeometryDim::D3>>(
				field,
				gradField,
				mesh
			);
	};

	void compute(
		const Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* field,
		Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>* gradField,
		const Mesh<MeshDim::D3>* mesh
	) override 
	{
		static_cast<Derived*>(this)->
			template computeImpl<Vector<GeometryDim::D3>, MatrixTensor<GeometryDim::D3>>(
				field,
				gradField,
				mesh
			);
	};
};

namespace gradUtils {

	using V = Vector<GeometryDim::D3>;
	using T = MatrixTensor<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;

	template<typename Obj, typename GradObj>
	__device__
		GradObj interpolateOnFace(
			uint32_t faceID,
			const Field<Obj, C>* field,
			Field<GradObj, C>* gradField,
			const Mesh<MeshDim::D3>* mesh);
}