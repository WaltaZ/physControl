#include <simulation/discretization/convection/convectionQUICK.h>

// -------------------- Assemble Inner Implementation ----------------------

template<typename Obj, typename GradObj>
__device__ void ConvectionQUICK::assembleInnerImpl(
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

	ConvectionUpwind upwind{};

	upwind.assembleInner(
		mesh,
		field,
		gradField,
		massFlowRateField,
		matrix
	);

	Obj B_contribution{};

	for (size_t i = 0; i < cell.cellFaceIDs.length; i++)
	{
		uint32_t faceID = cell.cellFaceIDs[i];
		const auto& face = mesh->faces[faceID];
		uint32_t neighbourID = face.getNeighbourCellID(id);

		if (face.isBoundary) { continue; }

		double massFlowRate = massFlowRateField->values[faceID];
		const Obj& phi_C = field->values[id];
		const GradObj& gradPhi_C = gradField->values[id];

		const Obj& phi_F = field->values[neighbourID];

		Obj upwindExplicit = 
			(phi_C * (-std::max(massFlowRate, 0.0))) +
			(phi_F * std::max(-massFlowRate, 0.0));

		GradObj gradFace = gradUtils::interpolateOnFace(faceID, field, gradField, mesh);

		Obj highResolutionExplicit = 
		(
			phi_C
			+ (geomOp::dotProduct(
				gradPhi_C + gradFace,
				face.getCellData(id).centroidToFace.vector
			) * 0.5)
		)
		* massFlowRate;

		B_contribution = B_contribution - upwindExplicit - highResolutionExplicit;
	}

	matrix->contributeTo_B(B_contribution);
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