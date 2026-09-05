#pragma once

#include "unsteady.h"

class UnsteadyEulerBackward : public Unsteady<UnsteadyEulerBackward> {
public:
	template<class Obj>
	__device__
		void assembleImpl(
			CudaMesh<MeshDim::D3>* mesh,
			CudaField<Obj, C>* field,
			CudaLinearSolverMatrix<Obj>* matrix,
			double delta_t
		);
};