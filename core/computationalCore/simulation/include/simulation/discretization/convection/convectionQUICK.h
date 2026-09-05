#pragma once

#include "convection.h"
#include "convectionUpwind.h"
#include <simulation/discretization/gradient/gradient.h>

class ConvectionQUICK : public Convection<ConvectionQUICK> {
public:
	template<typename Obj, typename GradObj>
	__device__ void assembleInnerImpl(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<Obj, C>* field,
		CudaField<GradObj, C>* gradField,
		CudaField<double, F>* massFlowRateField,
		CudaLinearSolverMatrix<Obj>* matrix
	);

	template<typename Obj, typename GradObj>
	__device__ void assembleBoundariesImpl(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<Obj, C>* field,
		CudaField<GradObj, C>* gradField,
		CudaField<double, F>* massFlowRateField,
		CudaLinearSolverMatrix<Obj>* matrix
	);
};