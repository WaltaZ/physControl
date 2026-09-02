#include <simulation/discretization/diffusion/diffusion.h>

__device__
void SimpleDiffusion::assembleInner(
	CudaLinearSolverMatrix* matrix,
	CudaMesh<MeshDim::D3>* mesh,
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
