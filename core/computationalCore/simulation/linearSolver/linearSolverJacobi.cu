#include <simulation/linearSolver/linearSolverJacobi.h>

#include <utility/cudaUtils.h>

constexpr int THREADS_PER_BLOCK = 64;

namespace jacobiKernels {
	template<class Obj>
	__global__
	void _nextStep(
		LinearSolverMatrix<Obj>* matrix,
		Obj* newPhi, 
		Field<Obj, Cell<MeshDim::D3>>* field,
		const Mesh<MeshDim::D3>* mesh)
	{
		int tId = threadIdx.x;
		int bId = blockIdx.x;

		int globId = bId * blockDim.x + tId;
		int bNum = ceil((float)field->values.length / (float)blockDim.x);

		const auto& A_F = matrix->A_F[globId];
		const auto& A_C = matrix->A_C[globId];
		const auto& B = matrix->B[globId];

		const auto& C = mesh->cells[globId];

		uint32_t FIdsLength = C.cellNeighbourCells.length;
		uint32_t* FIds = new uint32_t[FIdsLength];

		for (size_t i = 0; i < FIdsLength; i++)
		{
			FIds[i] = C.cellNeighbourCells[i]; // Bottle neck. Will parallelize that in the future.
		};

		__shared__ Obj s_phi[THREADS_PER_BLOCK];

		Obj sum_a_times_phi{};

		uint32_t phiLength = field->values.length;

		for (size_t phase = 0; phase < bNum; phase++)
		{
			__syncthreads();

			int id = phase * blockDim.x + tId;

			if (id < phiLength) {
				s_phi[tId] = field->values[id];
			}
			else {
				s_phi[tId] = Obj();
			}

			__syncthreads();

			if (id >= phiLength) { continue; }

			for (size_t FId = 0; FId < FIdsLength; FId++)
			{
				if (FIds[FId] > id) { continue; }
				sum_a_times_phi += A_F[FId] * s_phi[tId];
			}

		}

		newPhi[globId] = (B - sum_a_times_phi) / A_C;

		delete[] FIds;
	}

	template
		__global__
		void _nextStep(
			LinearSolverMatrix<double>* matrix,
			double* newPhi,
			Field<double, Cell<MeshDim::D3>>* field,
			const Mesh<MeshDim::D3>* mesh);

	template
		__global__
		void _nextStep(
			LinearSolverMatrix<Vector<GeometryDim::D3>>* matrix,
			Vector<GeometryDim::D3>* newPhi,
			Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* field,
			const Mesh<MeshDim::D3>* mesh);
}

template<class Obj>
LinearSolverJacobi<Obj>::LinearSolverJacobi(
	Mesh<MeshDim::D3>* mesh, 
	Field<Obj, Cell<MeshDim::D3>>* field) 
	: LinearSolver<Obj>(mesh, field) {};

template<class Obj>
void LinearSolverJacobi<Obj>::solve() {

	using namespace jacobiKernels;

	Obj* newPhi;
	cudaMalloc(&newPhi, _field->values.length * sizeof(Obj));

	KernelArgs args = cudaUtils::getKernelArgs(_field->values.length, THREADS_PER_BLOCK);
	
	_nextStep<<<args.blocks, args.threads>>>(matrix, newPhi, _field, _mesh);
}

template class LinearSolverJacobi<double>;
template class LinearSolverJacobi<Vector<GeometryDim::D3>>;