#include <simulation/discretization/convection/convectionQUICK.h>

// -------------------- Assemble Inner Implementation ----------------------

template<typename Obj, typename GradObj>
__device__ void ConvectionQUICK::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<Obj, C>* field,
	CudaField<GradObj, C>* gradField,
	CudaField<double, F>* massFlowRate,
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


	}
};

template
__device__ void ConvectionQUICK::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<double, C>* field,
	CudaField<V, C>* gradField,
	CudaField<double, F>* massFlowRate,
	CudaLinearSolverMatrix<double>* matrix
);

template
__device__ void ConvectionQUICK::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<V, C>* field,
	CudaField<T, C>* gradField,
	CudaField<double, F>* massFlowRate,
	CudaLinearSolverMatrix<V>* matrix
);