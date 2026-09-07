#pragma once

#include "convection.h"

class ConvectionUpwind : public Convection<ConvectionUpwind> {
public:
	template<typename Obj, typename GradObj>
	__device__ void assembleInnerImpl(
		const Mesh<MeshDim::D3>* mesh,
		Field<Obj, C>* field,
		Field<GradObj, C>* gradField,
		Field<double, F>* massFlowRate,
		LinearSolverMatrix<Obj>* matrix
	);

	template<typename Obj, typename GradObj>
	__device__ void assembleBoundariesImpl(
		const Mesh<MeshDim::D3>* mesh,
		Field<Obj, C>* field,
		Field<GradObj, C>* gradField,
		Field<double, F>* massFlowRate,
		LinearSolverMatrix<Obj>* matrix
	);

};