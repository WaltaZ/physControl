#pragma once

#include "diffusion.h"
#include <utility/mathUtils.h>

class DiffusionSimple : public Diffusion<DiffusionSimple> {
public:
	template<typename Obj>
	__device__ void assembleInnerImpl(
		const Mesh<MeshDim::D3>* mesh,
		Field<Obj, C>* field,
		LinearSolverMatrix<Obj>* matrix,
		double diffCoeff
	);

	template<typename Obj>
	__device__ void assembleBoundariesImpl(
		const Mesh<MeshDim::D3>* mesh,
		Field<Obj, C>* field,
		LinearSolverMatrix<Obj>* matrix,
		double diffCoeff
	);
};