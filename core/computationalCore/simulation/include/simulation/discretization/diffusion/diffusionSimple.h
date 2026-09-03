#pragma once

#include "diffusion.h"

class SimpleDiffusion : Diffusion<SimpleDiffusion> {
public:
	template<typename Obj>
	__device__ void assembleInnerImpl(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<Obj, C>* field,
		CudaLinearSolverMatrix<Obj>* matrix,
		double diffCoeff
	);

	template<typename Obj>
	__device__ void assembleBoundariesImpl(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<Obj, C>* field,
		CudaLinearSolverMatrix<Obj>* matrix,
		double diffCoeff
	);
};