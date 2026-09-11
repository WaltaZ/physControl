#include <simulation/discretization/relaxation/relaxationPatankar.h>

template<class Obj>
__device__
void RelaxationPatankar::relax(
	LinearSolverMatrix<Obj>* matrix,
	Field<Obj, Cell<MeshDim::D3>>* field,
	double relaxCoeff
) 
{
	int C_id = blockDim.x * blockIdx.x + threadIdx.x;

	if (C_id >= field->getLength()) { return; }

	auto& A_C = matrix->A_C[C_id];
	auto& B = matrix->B[C_id];
	const auto& phi = field->values[C_id];

	B += ((1 - relaxCoeff) / relaxCoeff) * A_C * phi;
	A_C /= relaxCoeff;
};

template
__device__
void RelaxationPatankar::relax(
	LinearSolverMatrix<double>* matrix,
	Field<double, Cell<MeshDim::D3>>* field,
	double relaxCoeff
);

template
__device__
void RelaxationPatankar::relax(
	LinearSolverMatrix<Vector<GeometryDim::D3>>* matrix,
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* field,
	double relaxCoeff
);

