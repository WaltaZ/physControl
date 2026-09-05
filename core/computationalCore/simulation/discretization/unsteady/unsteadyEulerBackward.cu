#include <simulation/discretization/unsteady/unsteadyEulerBackward.h>

template<class Obj>
__device__
void UnsteadyEulerBackward::assembleImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<Obj, C>* field,
	CudaLinearSolverMatrix<Obj>* matrix,
	double delta_t
) 
{
	assert(field->pastValues.length > 0);

	int C_id = blockIdx.x * blockDim.x + threadIdx.x;

	if (C_id >= mesh->cells.length) { return; }

	const auto& C = mesh->cells[C_id];
	const Obj& phi_past = field->pastValues[0][C_id];

	double v_over_dt = C.volume / delta_t;

	matrix->A_C[C_id] = matrix->A_C[C_id] + v_over_dt;
	matrix->B[C_id] = matrix->B[C_id] + (v_over_dt * phi_past);
};

template __device__
void UnsteadyEulerBackward::assembleImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<double, C>* field,
	CudaLinearSolverMatrix<double>* matrix,
	double delta_t
);

template __device__
void UnsteadyEulerBackward::assembleImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<V, C>* field,
	CudaLinearSolverMatrix<V>* matrix,
	double delta_t
);