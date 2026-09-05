#pragma once

#include "diffusion.h"
#include <utility/mathUtils.h>

class DiffusionSimple : public Diffusion<DiffusionSimple> {
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