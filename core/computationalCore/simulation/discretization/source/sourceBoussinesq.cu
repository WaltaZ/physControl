#include <simulation/discretization/source/sourceBoussinesq.h>

__device__
void SourceBoussinesq::assembleInner(
	CudaMesh<MeshDim::D3>* mesh,
	CudaLinearSolverMatrix<V>* matrix,
	V g,
	CudaField<double, C>* temperatureField,
	const double T_0,
	const double beta
) 
{
	int C_id = blockIdx.x * blockDim.x + threadIdx.x;

	if (C_id >= mesh->cells.length) { return; }

	const auto& C = mesh->cells[C_id];
	const double T_C = temperatureField->values[C_id];

	matrix->B[C_id] = 
		matrix->B[C_id] + 
		(
			(g * C.volume * beta)
			* (T_0 - T_C)
		);
};