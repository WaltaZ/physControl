#include <simulation/discretization/source/sourceGravity.h>

__device__
void SourceGravity::assembleInner(
	const Mesh<MeshDim::D3>* mesh,
	LinearSolverMatrix<V>* matrix,
	V g
) 
{
	int C_id = blockIdx.x * blockDim.x + threadIdx.x;

	if (C_id >= mesh->cells.length) { return; }

	const auto& C = mesh->cells[C_id];

	matrix->B[C_id] = matrix->B[C_id] + (g * C.volume);
};