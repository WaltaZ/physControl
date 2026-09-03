#include <simulation/discretization/diffusion/diffusionSimple.h>

// ------------------------------ Assemble Inner Faces Implementation ----------------------------

template<typename Obj>
__device__
void SimpleDiffusion::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<Obj, C>* field,
	CudaLinearSolverMatrix<Obj>* matrix,
	double diffCoeff) 
{
	int id = threadIdx.x + blockDim.x * blockIdx.x;

	const auto& cell = mesh->cells[id];

	double A_C_contribution = 0;

	for (int i = 0; i < matrix->A_F[id].length; i++) {
		const auto& face = mesh->faces[cell.cellFaceIDs[i]];

		if (!face.isBoundary) {
			double A_F_contribution = 
				-diffCoeff * 
				face.getArea(id).magnitude / 
				face.getCellData(id).centroidToFace.magnitude;

			A_C_contribution += -A_F_contribution;

			matrix->A_F[id][i] += A_F_contribution;
		}
	}
	matrix->A_C[id] += A_C_contribution;
}

template void SimpleDiffusion::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<double, C>* field,
	CudaLinearSolverMatrix<double>* matrix,
	double diffCoeff);

template void SimpleDiffusion::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<V, C>* field,
	CudaLinearSolverMatrix<V>* matrix,
	double diffCoeff);

// ------------------------------ Assemble Boundary Faces Implementation ----------------------------

template<typename Obj>
__device__
void SimpleDiffusion::assembleBoundariesImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<Obj, C>* field,
	CudaLinearSolverMatrix<Obj>* matrix,
	double diffCoeff
) 
{
	int id = threadIdx.x + blockDim.x * blockIdx.x;

	//const auto& boundaryPatches = 
}

template void SimpleDiffusion::assembleBoundariesImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<double, C>* field,
	CudaLinearSolverMatrix<double>* matrix,
	double diffCoeff);

template void SimpleDiffusion::assembleBoundariesImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<V, C>* field,
	CudaLinearSolverMatrix<V>* matrix,
	double diffCoeff);