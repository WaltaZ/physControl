#pragma once

#include "unsteady.h"

class UnsteadyEulerBackward : public Unsteady<UnsteadyEulerBackward> {
public:
	template<class Obj>
	__device__
		void assembleImpl(
			const Mesh<MeshDim::D3>* mesh,
			Field<Obj, C>* field,
			LinearSolverMatrix<Obj>* matrix,
			double delta_t
		);
};