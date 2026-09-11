#include <simulation/discretization/source/sourceBoussinesq.h>

__device__
void SourceBoussinesq::assembleInner(
	const Mesh<MeshDim::D3>* mesh,
	LinearSolverMatrix<V>* matrix,
	V g,
	Field<double, C>* temperatureField,
	const double beta
) 
{
	int C_id = blockIdx.x * blockDim.x + threadIdx.x;

	if (C_id >= mesh->cells.length) { return; }

	const auto& C = mesh->cells[C_id];
	const double T_C = temperatureField->values[C_id];

	const double T_0 = 0.5 * (temperatureField->maxValue + temperatureField->minValue);

	matrix->B[C_id] += 
		(
			(g * C.volume * beta)
			* (T_0 - T_C)
		);
};