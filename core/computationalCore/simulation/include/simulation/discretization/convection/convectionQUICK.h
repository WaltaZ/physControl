#pragma once

#include "convection.h"

class ConvectionQUICK : public Convection<ConvectionQUICK> {

	template<typename Obj, typename GradObj>
	__device__ void assembleInnerImpl(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<Obj, C>* field,
		CudaField<GradObj, C>* gradField,
		CudaField<double, F>* massFlowRate,
		CudaLinearSolverMatrix<Obj>* matrix
	);

	template<typename Obj, typename GradObj>
	__device__ void assembleBoundariesImpl(
		CudaMesh<MeshDim::D3>* mesh,
		CudaField<Obj, C>* field,
		CudaField<GradObj, C>* gradField,
		CudaField<double, F>* massFlowRate,
		CudaLinearSolverMatrix<Obj>* matrix
	);
};