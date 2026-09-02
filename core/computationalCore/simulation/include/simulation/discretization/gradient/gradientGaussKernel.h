#pragma once

#include <problem/field.h>
#include <mesh/meshElements/meshElements.h>
#include <geometry/geometry.h>
#include <utility/cudaUtils.h>
#include <utility/utility.h>

namespace CUDA_GradientGauss {

	template<typename Obj, typename ObjDest>
	__global__ void CUDA_compute_EC_internalFaces(
		CudaField<Obj, Cell<MeshDim::D3>>* field,
		CudaField<ObjDest, Cell<MeshDim::D3>>* destField,
		CudaMesh<MeshDim::D3>* mesh
	);

	__global__ void CUDA_compute_EF_noBC(
		CudaField<double, Cell<MeshDim::D3>>* field,
		CudaField<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* destField,
		CudaMesh<MeshDim::D3>* mesh
	);

	__global__ void CUDA_compute_EF_BC(
		CudaField<double, Cell<MeshDim::D3>>* field,
		CudaField<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* destField,
		CudaMesh<MeshDim::D3>* mesh
	);
}