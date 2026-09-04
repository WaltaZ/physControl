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
	int id = threadIdx.x + blockDim.x * blockIdx.x; // Id of a cell

	if (id >= mesh->cells.length) { return; }

	const auto& cell = mesh->cells[id];

	double A_C_contribution = 0;

	for (size_t i = 0; i < cell.cellFaceIDs.length; i++)
	{
		uint32_t faceID = cell.cellFaceIDs[i];
		const auto& face = mesh->faces[faceID];

		if (face.isBoundary) { continue; }

		double massFlowRate = massFlowRateField->values[faceID];

		// Page 410 from the book
		matrix->A_F[id][i] = -std::max(-massFlowRate, 0.0);
		A_C_contribution += std::max(massFlowRate, 0.0);
	}

	matrix->contributeTo_A_C(id, A_C_contribution);

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