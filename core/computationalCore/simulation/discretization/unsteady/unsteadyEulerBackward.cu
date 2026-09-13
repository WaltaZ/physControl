#include <simulation/discretization/unsteady/unsteadyEulerBackward.h>

template<class Obj>
__device__
void UnsteadyEulerBackward::assembleImpl(
	const Mesh<MeshDim::D3>* mesh,
	Field<Obj, C>* field,
	LinearSolverMatrix<Obj>* matrix,
	double delta_t
) 
{
	assert(field->pastValues.length > 0);

	int C_id = blockIdx.x * blockDim.x + threadIdx.x;


	if (C_id >= mesh->cells.length) { return; }

	const auto& C = mesh->cells[C_id];
	const Obj& phi_past = field->pastValues[0][C_id];

	double v_over_dt = C.volume / delta_t;

	matrix->A_C[C_id] += Obj{ v_over_dt };
	matrix->B[C_id] += (v_over_dt * phi_past);
};

template __device__
void UnsteadyEulerBackward::assembleImpl(
	const Mesh<MeshDim::D3>* mesh,
	Field<double, C>* field,
	LinearSolverMatrix<double>* matrix,
	double delta_t
);

template __device__
void UnsteadyEulerBackward::assembleImpl(
	const Mesh<MeshDim::D3>* mesh,
	Field<V, C>* field,
	LinearSolverMatrix<V>* matrix,
	double delta_t
);