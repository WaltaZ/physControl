#pragma once

#include <simulation/discretization/gradient/gradient.h>
#include <cuda_runtime.h>

namespace SimpleGradientCuda {

	__global__ void _compute_EC_internalFaces(
		double* value,
		Vector<GeometryDim::D3>* destVector,
		CudaMesh<MeshDim::D3>* mesh);
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