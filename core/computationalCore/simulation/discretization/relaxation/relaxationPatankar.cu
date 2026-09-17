#include <simulation/discretization/relaxation/relaxationPatankar.h>

namespace relaxation {
	template<class Obj>
	__device__
		void patankar(
			LinearSolverMatrix<Obj>* matrix,
			Field<Obj, Cell<MeshDim::D3>>* field,
			double relaxCoeff
		)
	{
		int C_id = blockDim.x * blockIdx.x + threadIdx.x;

		if (C_id >= field->getLength()) { return; }

		auto& A_C = matrix->A_C[C_id];
		auto& B = matrix->B[C_id];
		const auto& phi_old = field->pastValues[0][C_id];

		B += ((1 - relaxCoeff) / relaxCoeff) * geomOp::hadProduct(A_C, phi_old);
		A_C /= relaxCoeff;
	};

	template
		__device__
		void patankar(
			LinearSolverMatrix<double>* matrix,
			Field<double, Cell<MeshDim::D3>>* field,
			double relaxCoeff
		);

	template
		__device__
		void patankar(
			LinearSolverMatrix<Vector<GeometryDim::D3>>* matrix,
			Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* field,
			double relaxCoeff
		);
}