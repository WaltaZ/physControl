#pragma once

#include <utility/cudaUtils.h>
#include <mesh/meshElements/mesh.h>

template<typename Obj>
class LinearSolverMatrix {
public:
	CudaPackedArray<double> A_C;
	CudaPackedArray<CudaArray<double>> A_F;
	CudaPackedArray<Obj> B;

	LinearSolverMatrix(const Mesh<MeshDim::D3>* mesh);
	
private:
	CudaPackedArray<double> A_Fs;
};