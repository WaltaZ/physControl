#include <simulation/discretization/diffusion/diffusionSimple.h>

// ------------------------------ Assemble Inner Faces Implementation ----------------------------

// TODO: Check if it works

template<typename Obj>
__device__
void DiffusionSimple::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<Obj, C>* field,
	CudaLinearSolverMatrix<Obj>* matrix,
	double diffCoeff) 
{
	int C_id = blockIdx.x * blockDim.x + threadIdx.x;;

	const auto& cell = mesh->cells[C_id];

	double A_C_contribution = 0;

	for (int i = 0; i < matrix->A_F[C_id].length; i++) {
		const auto& face = mesh->faces[cell.cellFaceIDs[i]];

		if (!face.isBoundary) {
			double A_F_contribution = 
				-diffCoeff * 
				face.getArea(C_id).magnitude / 
				face.getCellData(C_id).centroidToFace.magnitude;

			A_C_contribution += -A_F_contribution;

			matrix->A_F[C_id][i] += A_F_contribution;
		}
	}
	matrix->A_C[C_id] = A_C_contribution;
}

template void DiffusionSimple::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<double, C>* field,
	CudaLinearSolverMatrix<double>* matrix,
	double diffCoeff);

template void DiffusionSimple::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<V, C>* field,
	CudaLinearSolverMatrix<V>* matrix,
	double diffCoeff);

// ------------------------------ Assemble Boundary Faces Implementation ----------------------------

template<typename Obj>
__device__
void DiffusionSimple::assembleBoundariesImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<Obj, C>* field,
	CudaLinearSolverMatrix<Obj>* matrix,
	double diffCoeff
) 
{
	int bp_faceId = blockIdx.x * blockDim.x + threadIdx.x; // index of an array faceIDs inside a patch

	auto& boundaryPatches = field->boundaryPatches;

	for (size_t patchNum = 0; patchNum < boundaryPatches.length; patchNum++)
	{
		const auto& bp = boundaryPatches[patchNum];

		if (bp_faceId >= bp.faceIDs.length) { continue; }

		// Mesh elements
		const auto& f = mesh->faces[bp.faceIDs[bp_faceId]];
		const auto& C_id = f.ownerCellID;
		const auto& C = mesh->cells[C_id];

		// Temporary walkaround for not refactoring everything again. BoundaryPatch has a value field
		// which is supposed to operate not only on doubles. What if the boundary value is a vector?
		// defenitely something to be fixed in the future, but for now the convention is following:
		// 
		// FIRST indices are a boundary value (like a dobule, a vector) and the rest are supporting value.
		// Example: suppose we have a mixed boundary condition with a phi_infty vector and a h_infty scalar.
		// Then the boundary patch value field is encoded in the following way:
		// 
		// value[0] = phi_infty.comp[0]
		// value[1] = phi_infty.comp[1]
		// value[2] = phi_infty.comp[2]
		// value[3] = h_infty

		double A_C_contribution = 0;
		// double A_F_contribution = 0; -> This is always equal to zero
		Obj B_contribution{};

		const double* bp_values = bp.values.getData();
		Obj phi_b = mathUtils::createMathObj<Obj>(bp_values);
		uint32_t phi_b_numOfComp = mathUtils::getNumOfComp(phi_b);

		switch (bp.type) {
		case BoundaryConditionType::Drichlet:
			A_C_contribution =
				(
					f.area.magnitude 
					/ f.ownerData.centroidToFace.magnitude
				) 
				* diffCoeff;

			B_contribution = A_C_contribution * phi_b;
			break;

		case BoundaryConditionType::Neumann:
			B_contribution = -f.area.magnitude * phi_b;
			break;

		case BoundaryConditionType::Mixed:
			double h_infty = bp_values[phi_b_numOfComp];
			double gamma_b = bp_values[phi_b_numOfComp + 1];

			double gamma_over_d =
				(
					gamma_b
					/ f.ownerData.centroidToFace.magnitude
					);
			double Req = 
			(
				(h_infty * gamma_over_d) / (h_infty + gamma_over_d)
			)
			*
			f.area.magnitude;

			A_C_contribution = Req;
			B_contribution = -Req * phi_b;
			break;

		default:
			continue;
		}
		
		cudaUtils::contributeTo(matrix->A_C[C_id], A_C_contribution);
		cudaUtils::contributeTo(matrix->B[C_id], B_contribution);
	}
}

template void DiffusionSimple::assembleBoundariesImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<double, C>* field,
	CudaLinearSolverMatrix<double>* matrix,
	double diffCoeff);

template void DiffusionSimple::assembleBoundariesImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<V, C>* field,
	CudaLinearSolverMatrix<V>* matrix,
	double diffCoeff);