#pragma once

#include <cuda_runtime.h>
#include <mesh/meshElements/mesh.h>
#include <simulation/linearSolver/linearSolverMatrix.h>
#include <geometry/matrixTensor.h>

class Diffusion {
	__device__ virtual void assembleInner(
		CudaLinearSolverMatrix* matrix,
		CudaMesh<MeshDim::D3>* mesh,
		double diffCoeff
	) = 0;

	__device__ virtual void assembleBoundaries(
		CudaLinearSolverMatrix* matrix,
		CudaMesh<MeshDim::D3>* mesh,
		double diffCoeff
	) = 0;
};

class SimpleDiffusion : public Diffusion {
	__device__ void assembleInner(
		CudaLinearSolverMatrix* matrix,
		CudaMesh<MeshDim::D3>* mesh,
		double diffCoeff
	) override;

	__device__ void assembleBoundaries(
		CudaLinearSolverMatrix* matrix,
		CudaMesh<MeshDim::D3>* mesh,
		double diffCoeff
	);
};