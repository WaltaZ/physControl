#pragma once

#include <utility/cudaUtils.h>
#include <mesh/meshElements/mesh.h>

template<typename Obj>
class CudaLinearSolverMatrix {
public:
	CudaPackedArray<double> A_C;
	CudaPackedArray<CudaArray<double>> A_F;
	CudaPackedArray<Obj> B;

	CudaLinearSolverMatrix(const Mesh<MeshDim::D3>& mesh);
	
private:
	CudaPackedArray<double> A_Fs;
};

template<typename Obj>
class LinearSolverMatrix {
public:
	LinearSolverMatrix(const Mesh<MeshDim::D3>& mesh);
	CudaLinearSolverMatrix<Obj>* getElements();
	const CudaLinearSolverMatrix<Obj>* getElements() const;
private:
	CudaLinearSolverMatrix<Obj>* _linearSolverMatrix;
};