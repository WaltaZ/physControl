#include <simulation/discretization/convection/convectionUpwind.h>
#include <algorithm>

// -------------------- Assemble Inner Implementation ----------------------

template<typename Obj, typename GradObj>
__device__ void ConvectionUpwind::assembleInnerImpl(
	const Mesh<MeshDim::D3>* mesh,
	Field<Obj, C>* field,
	Field<GradObj, C>* gradField,
	Field<double, F>* massFlowRateField,
	LinearSolverMatrix<Obj>* matrix
)
{
	int C_id = blockIdx.x * blockDim.x + threadIdx.x;

	if (C_id >= mesh->cells.length) { return; }

	const auto& C = mesh->cells[C_id];

	auto& A_C = matrix->A_C[C_id];
	auto& A_F = matrix->A_F[C_id];

	Obj A_C_contribution{};

	for (size_t i = 0; i < A_F.length; i++)
	{
		uint32_t f_id = mesh->getCommonFaceId(C, i);
		const auto& f = mesh->faces[f_id];

		if (f.isBoundary) { continue; }

		double m_f = massFlowRateField->values[f_id];
		if (C_id == f.ownerCellID) { m_f = -m_f; }

		// Page 410 from the book
		A_F[i] -= Obj{ std::max(-m_f, 0.0) };
		A_C_contribution += Obj{ std::max(m_f, 0.0) };
	}

	A_C += A_C_contribution;

	// No B contribution
};

template
__device__ void ConvectionUpwind::assembleInnerImpl(
	const Mesh<MeshDim::D3>* mesh,
	Field<double, C>* field,
	Field<V, C>* gradField,
	Field<double, F>* massFlowRate,
	LinearSolverMatrix<double>* matrix
);

template
__device__ void ConvectionUpwind::assembleInnerImpl(
	const Mesh<MeshDim::D3>* mesh,
	Field<V, C>* field,
	Field<T, C>* gradField,
	Field<double, F>* massFlowRate,
	LinearSolverMatrix<V>* matrix
);

// -------------------- Assemble Boundaries Implementation ----------------------

template<typename Obj, typename GradObj>
__device__ void ConvectionUpwind::assembleBoundariesImpl(
	const Mesh<MeshDim::D3>* mesh,
	Field<Obj, C>* field,
	Field<GradObj, C>* gradField,
	Field<double, F>* massFlowRateField,
	LinearSolverMatrix<Obj>* matrix
) 
{}; // TODO: Implement

template
__device__ void ConvectionUpwind::assembleBoundariesImpl(
	const Mesh<MeshDim::D3>* mesh,
	Field<double, C>* field,
	Field<V, C>* gradField,
	Field<double, F>* massFlowRate,
	LinearSolverMatrix<double>* matrix
);

template
__device__ void ConvectionUpwind::assembleBoundariesImpl(
	const Mesh<MeshDim::D3>* mesh,
	Field<V, C>* field,
	Field<T, C>* gradField,
	Field<double, F>* massFlowRate,
	LinearSolverMatrix<V>* matrix
);