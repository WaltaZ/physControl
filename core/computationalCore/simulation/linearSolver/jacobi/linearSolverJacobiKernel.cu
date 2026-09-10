#include <simulation/linearSolver/jacobi/linearSolverJacobiKernel.h>

#include <simulation/linearSolver/jacobi/linearSolverJacobiConfig.h>

namespace jacobiKernels {

	// --------------------------------------- Next step ----------------------------------------

	template<class Obj>
	__global__
		void _nextStep(
			Obj* newPhi,
			Field<Obj, Cell<MeshDim::D3>>* field,
			LinearSolverMatrix<Obj>* matrix,
			const Mesh<MeshDim::D3>* mesh)
	{
		int C_id = blockIdx.x * blockDim.x + threadIdx.x;

		if (C_id >= field->getLength()) { return; }

		const auto& C = mesh->cells[C_id];

		const auto& A_C = matrix->A_C[C_id];
		const auto& A_F = matrix->A_F[C_id];
		const auto& B = matrix->B[C_id];

		Obj sum_AF_times_phi{};

		for (size_t i = 0; i < A_F.length; i++)
		{
			const Obj& phi_F = field->values[C.cellNeighbourCells[i]];
			sum_AF_times_phi += A_F[i] * phi_F;
		}

		newPhi[C_id] = (B - sum_AF_times_phi) / A_C;
	}

	template
		__global__
		void _nextStep(
			double* newPhi,
			Field<double, Cell<MeshDim::D3>>* field,
			LinearSolverMatrix<double>* matrix,
			const Mesh<MeshDim::D3>* mesh);

	template
		__global__
		void _nextStep(
			Vector<GeometryDim::D3>* newPhi,
			Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* field,
			LinearSolverMatrix<Vector<GeometryDim::D3>>* matrix,
			const Mesh<MeshDim::D3>* mesh);

	// --------------------------------------- Update Field ----------------------------------------

	template<class Obj>
	__global__
		void _updateField(
			Obj* newPhi,
			Field<Obj, Cell<MeshDim::D3>>* field
		)
	{
		int C_id = blockIdx.x * blockDim.x + threadIdx.x;

		if (C_id >= field->getLength()) { return; }

		field->values[C_id] = newPhi[C_id];

	}

	template
		__global__
		void _updateField(
			double* newPhi,
			Field<double, Cell<MeshDim::D3>>* field
		);

	template
		__global__
		void _updateField(
			Vector<GeometryDim::D3>* newPhi,
			Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* field
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
		)
	{
		int C_id = blockIdx.x * blockDim.x + threadIdx.x;

		if (C_id >= newPhiLength) { return; }

		const auto& C = mesh->cells[C_id];

		const auto& A_C = matrix->A_C[C_id];
		const auto& A_F = matrix->A_F[C_id];
		const auto& B = matrix->B[C_id];

		const auto& phi = newPhi[C_id];

		Obj sum_A_times_phi{};

		for (size_t i = 0; i < A_F.length; i++)
		{
			const Obj& phi_F = newPhi[C.cellNeighbourCells[i]];
			sum_A_times_phi += A_F[i] * phi_F;
		}


		sum_A_times_phi += A_C * phi;

		r[C_id] = (B - sum_A_times_phi);
	}

	template
		__global__
		void _updateRisiduals(
			double* r,
			double* newPhi,
			uint32_t newPhiLength,
			LinearSolverMatrix<double>* matrix,
			const Mesh<MeshDim::D3>* mesh
		);

	template
		__global__
		void _updateRisiduals(
			Vector<GeometryDim::D3>* r,
			Vector<GeometryDim::D3>* newPhi,
			uint32_t newPhiLength,
			LinearSolverMatrix<Vector<GeometryDim::D3>>* matrix,
			const Mesh<MeshDim::D3>* mesh
		);

	// -------------------------------------- Norm of the residual ------------------------------------------

	__device__
		void _copyDataIntoShared(
			uint32_t globId,
			double& s_r,
			double* scalar
		)
	{
		s_r = scalar[globId] * scalar[globId];
	}

	__device__
		void _copyDataIntoShared(
			uint32_t globId,
			double& s_r,
			Vector<GeometryDim::D3>* vec
		)
	{
		uint32_t vecId = globId / 3;
		uint32_t comp = globId % 3;

		s_r = (vec[vecId].comp[comp]) * (vec[vecId].comp[comp]);
	}

	template<class Obj>
	__global__
		void _addSquares(
			Obj* r,
			double* normR,
			uint32_t numOfAdditions
		)
	{
		int tId = threadIdx.x;
		int bId = blockIdx.x;

		int globId = bId * blockDim.x + tId;

		__shared__ double s_r[jacobiConfig::THREADS_PER_BLOCK];

		if (globId < numOfAdditions) {
			_copyDataIntoShared(globId, s_r[tId], r);
		}
		else {
			s_r[tId] = 0;
		}

		__syncthreads();

		for (size_t s = 1; s < blockDim.x; s *= 2)
		{
			int index = 2 * s * tId;
			if (index < blockDim.x)
			{
				s_r[index] += s_r[index + s];
			}

			__syncthreads();
		}

		if (tId == 0) {
			atomicAdd(normR, s_r[0]);
		}
	}

	template
		__global__
		void _addSquares(
			double* r,
			double* r_result,
			uint32_t numOfAdditions
		);

	template
		__global__
		void _addSquares(
			Vector<GeometryDim::D3>* r,
			double* r_result,
			uint32_t numOfAdditions
		);
}