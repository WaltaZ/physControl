#pragma once

#include <utility/cudaUtils.h>
#include <mesh/meshElements/mesh.h>

class CudaLinearSolverMatrix {
public:
	CudaAllocatedObj<double> A_C;
	CudaAllocatedObj<CudaArray<double>> A_F;
	CudaAllocatedObj<double> B;

	CudaLinearSolverMatrix(const Mesh<MeshDim::D3>& mesh);
private:
	CudaAllocatedObj<double> A_Fs;
};

class LinearSolverMatrix {
public:
	LinearSolverMatrix(const Mesh<MeshDim::D3>& mesh);
	CudaLinearSolverMatrix* getElements();
	const CudaLinearSolverMatrix* getElements() const;
private:
	CudaLinearSolverMatrix* _linearSolverMatrix;
};