#pragma once

#include <problem/field.h>
#include <mesh/meshElements/meshElements.h>
#include <geometry/geometry.h>
#include <utility/cudaUtils.h>
#include <utility/utility.h>

namespace CUDA_GradientGauss {

	using V = Vector<GeometryDim::D3>;
	using T = MatrixTensor<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;

	template<typename Obj, typename ObjDest>
	__global__ void CUDA_compute_EC_internalFaces(
		CudaField<Obj, C>* field,
		CudaField<ObjDest, C>* destField,
		CudaMesh<MeshDim::D3>* mesh
	);

	template<typename Obj, typename ObjDest>
	__global__ void CUDA_compute_EF_noBC(
		CudaField<Obj, C>* field,
		CudaField<ObjDest, C>* destField,
		CudaMesh<MeshDim::D3>* mesh
	);

	template<typename Obj, typename ObjDest>
	__global__ void CUDA_compute_EF_BC(
		CudaField<Obj, Cell<MeshDim::D3>>* field,
		CudaField<ObjDest, Cell<MeshDim::D3>>* destField,
		CudaMesh<MeshDim::D3>* mesh
	);
}