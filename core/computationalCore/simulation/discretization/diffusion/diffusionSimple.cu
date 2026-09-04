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
	matrix->contributeToA(id, A_C_contribution);
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
	int id = threadIdx.x + blockDim.x * blockIdx.x; // index of an array faceIDs inside a patch

	auto& boundaryPatches = field->boundaryPatches;

	for (size_t patchNum = 0; patchNum < boundaryPatches.length; patchNum++)
	{
		const auto& bp = boundaryPatches[patchNum];

		if (id >= bp.faceIDs.length) { continue; }

		// Mesh elements
		const auto& face = mesh->faces[bp.faceIDs[id]];
		const auto& cellID = face.ownerCellID;
		const auto& cell = mesh->cells[cellID];

		// Getting all the components to iterate over
		Obj* obj = field->values.getData() + cellID;
		CudaArray<double> objComp = geomUtils::getComponents(obj);

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

		size_t objLength = objComp.length;

		double A_C_contribution = 0;
		// double A_F_contribution = 0; -> This is always equal to zero
		double* B_contribution = new double[objLength];

		switch (bp.type) {
		case BoundaryConditionType::Drichlet:
			A_C_contribution =
				(face.area.magnitude /
					face.ownerData.centroidToFace.magnitude) *
				diffCoeff;
			for (size_t comp = 0; comp < objLength; comp++)
			{
				B_contribution[comp] = A_C_contribution * bp.values[comp];
			}
			break;

		case BoundaryConditionType::Neumann:
			for (size_t comp = 0; comp < objLength; comp++)
			{
				B_contribution[comp] = -bp.values[comp] * face.area.magnitude;
			}
			break;

		case BoundaryConditionType::Mixed:
			double gamma_over_d = diffCoeff / face.ownerData.centroidToFace.magnitude;
			double Req = (bp.values[objLength] * gamma_over_d) / (bp.values[objLength] + gamma_over_d);

			A_C_contribution = Req;
			for (size_t comp = 0; comp < objLength; comp++)
			{
				B_contribution[comp] = Req * bp.values[comp];
			}
			break;

		default:
			delete[] B_contribution;
			continue;
		}
		
		matrix->contributeToA(cellID, A_C_contribution);
		matrix->contributeToB(cellID, B_contribution);

		delete[] B_contribution;
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