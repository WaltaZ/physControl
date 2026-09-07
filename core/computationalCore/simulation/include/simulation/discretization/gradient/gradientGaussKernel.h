#pragma once

#include <problem/field.h>
#include <mesh/meshElements/meshElements.h>
#include <geometry/geometry.h>
#include <utility/cudaUtils.h>
#include <utility/utility.h>
#include <utility/mathUtils.h>

namespace CUDA_GradientGauss {

	using V = Vector<GeometryDim::D3>;
	using T = MatrixTensor<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;

	template<typename Obj, typename GradObj>
	__global__ void CUDA_compute_EC_internalFaces(
		const Field<Obj, C>* field,
		Field<GradObj, C>* gradField,
		const Mesh<MeshDim::D3>* mesh
	);

	template<typename Obj, typename GradObj>
	__global__ void CUDA_compute_EF_noBC(
		const Field<Obj, C>* field,
		Field<GradObj, C>* gradField,
		const Mesh<MeshDim::D3>* mesh
	);

	template<typename Obj, typename GradObj>
	__global__ void CUDA_compute_EF_BC(
		const Field<Obj, Cell<MeshDim::D3>>* field,
		Field<GradObj, Cell<MeshDim::D3>>* gradField,
		const Mesh<MeshDim::D3>* mesh
	);
}