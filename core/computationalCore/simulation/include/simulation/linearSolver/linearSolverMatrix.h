#pragma once

#include <utility/cudaUtils.h>
#include <mesh/meshElements/mesh.h>

template<typename Obj>
class LinearSolverMatrix {
public:
	CudaPackedArray<Obj> A_C;
	CudaPackedArray<CudaArray<Obj>> A_F;
	CudaPackedArray<Obj> B;

	LinearSolverMatrix(const Mesh<MeshDim::D3>* mesh);
	
private:
	CudaPackedArray<Obj> A_Fs;
};