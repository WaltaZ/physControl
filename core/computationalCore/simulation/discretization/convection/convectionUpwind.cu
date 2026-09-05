#include <simulation/discretization/convection/convectionUpwind.h>
#include <algorithm>

// -------------------- Assemble Inner Implementation ----------------------

template<typename Obj, typename GradObj>
__device__ void ConvectionUpwind::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<Obj, C>* field,
	CudaField<GradObj, C>* gradField,
	CudaField<double, F>* massFlowRateField,
	CudaLinearSolverMatrix<Obj>* matrix
)
{
	int C_id = threadIdx.x + blockDim.x * blockIdx.x; // Id of a cell

	if (C_id >= mesh->cells.length) { return; }

	const auto& C = mesh->cells[C_id];

	double A_C_contribution = 0;

	for (size_t i = 0; i < C.cellFaceIDs.length; i++)
	{
		uint32_t f_id = C.cellFaceIDs[i];
		const auto& f = mesh->faces[f_id];

		if (f.isBoundary) { continue; }

		double m_f = massFlowRateField->values[f_id];

		// Page 410 from the book
		matrix->A_F[C_id][i] = -std::max(-m_f, 0.0);
		A_C_contribution += std::max(m_f, 0.0);
	}

	matrix->A_C[C_id] = A_C_contribution;

	// No B contribution
};

template
__device__ void ConvectionUpwind::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<double, C>* field,
	CudaField<V, C>* gradField,
	CudaField<double, F>* massFlowRate,
	CudaLinearSolverMatrix<double>* matrix
);

template
__device__ void ConvectionUpwind::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<V, C>* field,
	CudaField<T, C>* gradField,
	CudaField<double, F>* massFlowRate,
	CudaLinearSolverMatrix<V>* matrix
);

// -------------------- Assemble Boundaries Implementation ----------------------

template<typename Obj, typename GradObj>
__device__ void ConvectionUpwind::assembleBoundariesImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<Obj, C>* field,
	CudaField<GradObj, C>* gradField,
	CudaField<double, F>* massFlowRateField,
	CudaLinearSolverMatrix<Obj>* matrix
) 
{}; // TODO: Implement

template
__device__ void ConvectionUpwind::assembleBoundariesImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<double, C>* field,
	CudaField<V, C>* gradField,
	CudaField<double, F>* massFlowRate,
	CudaLinearSolverMatrix<double>* matrix
);

template
__device__ void ConvectionUpwind::assembleBoundariesImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<V, C>* field,
	CudaField<T, C>* gradField,
	CudaField<double, F>* massFlowRate,
	CudaLinearSolverMatrix<V>* matrix
);