#pragma once

#include <simulation/discretization/gradient/gradient.h>
#include <cuda_runtime.h>

namespace CUDA_SimpleGradient {

	template<typename Obj, typename ObjDest>
	__global__ void CUDA_compute_EC_internalFaces(
		Obj* obj,
		ObjDest* destObj,
		CudaMesh<MeshDim::D3>* mesh
	);
	
	__global__ void CUDA_compute_EF_noBC(
		double* value,
		Vector<GeometryDim::D3>* destVector,
		CudaMesh<MeshDim::D3>* mesh
	);

	__global__ void CUDA_compute_EF_BC(
		double* value,
		Vector<GeometryDim::D3>* destVector,
		CudaMesh<MeshDim::D3>* mesh,
		BoundaryPatch* bp
	);
}

class SimpleGradient : public Gradient<MeshDim::D3> {	
public:
	void compute(
		Field<double, Cell<MeshDim::D3>>& field,
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
		Mesh<MeshDim::D3>& mesh
	) override;

	void compute(
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& field,
		Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
		Mesh<MeshDim::D3>& mesh
	) override;
};