#pragma once

#include "../linearSolverMatrix.h"

#include <mesh/meshElements/meshElements.h>
#include <problem/field/field.h>

namespace jacobiKernels {

	// --------------------------------------- Next step ----------------------------------------

	template<class Obj>
	__global__
		void _nextStep(
			Obj* newPhi,
			Field<Obj, Cell<MeshDim::D3>>* field,
			LinearSolverMatrix<Obj>* matrix,
			const Mesh<MeshDim::D3>* mesh);

	// --------------------------------------- Update Field ----------------------------------------

	template<class Obj>
	__global__
		void _updateField(
			Obj* newPhi,
			Field<Obj, Cell<MeshDim::D3>>* field
		);


	// --------------------------------------- Update Residual ----------------------------------------

	template<class Obj>
	__global__
		void _updateRisiduals(
			Obj* r,
			Obj* newPhi,
			uint32_t newPhiLength,
			LinearSolverMatrix<Obj>* matrix,
			const Mesh<MeshDim::D3>* mesh
		);

	// -------------------------------------- Norm of the residual ------------------------------------------

	__device__
		void _copyDataIntoShared(
			uint32_t globId,
			double& s_r,
			double* scalar
		);

	__device__
		void _copyDataIntoShared(
			uint32_t globId,
			double& s_r,
			Vector<GeometryDim::D3>* vec
		);

	template<class Obj>
	__global__
		void _addSquares(
			Obj* r,
			double* normR,
			uint32_t numOfAdditions
		);
}