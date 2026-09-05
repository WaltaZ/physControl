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
	int C_id = threadIdx.x + blockDim.x * blockIdx.x;

	if (C_id >= mesh->cells.length) { return; }

	const auto& C = mesh->cells[C_id];

	ConvectionUpwind upwind{};

	upwind.assembleInner(
		mesh,
		field,
		gradField,
		massFlowRateField,
		matrix
	);

	Obj B_contribution{};

	for (size_t i = 0; i < C.cellFaceIDs.length; i++)
	{
		uint32_t f_id = C.cellFaceIDs[i];
		const auto& f = mesh->faces[f_id];
		uint32_t F_id = f.getNeighbourCellID(C_id);

		if (f.isBoundary) { continue; }

		double m_f = massFlowRateField->values[f_id];
		const Obj& phi_C = field->values[C_id];
		const GradObj& gradPhi_C = gradField->values[C_id];

		const Obj& phi_F = field->values[F_id];

		Obj upwindExplicit = 
			(phi_C * (-std::max(m_f, 0.0))) +
			(phi_F * std::max(-m_f, 0.0));

		GradObj gradFace = gradUtils::interpolateOnFace(f_id, field, gradField, mesh);

		Obj highResolutionExplicit = 
		(
			phi_C
			+ 
			(0.5 * geomOp::dotProduct(
				gradPhi_C + gradFace,
				f.getCellData(C_id).centroidToFace.vector
			))
		)
		* m_f;

		B_contribution = B_contribution - upwindExplicit - highResolutionExplicit;
	}

	cudaUtils::contributeTo(matrix->B[C_id], B_contribution);
};

template
__device__ void ConvectionQUICK::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<double, C>* field,
	CudaField<V, C>* gradField,
	CudaField<double, F>* massFlowRateField,
	CudaLinearSolverMatrix<double>* matrix
);

template
__device__ void ConvectionQUICK::assembleInnerImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<V, C>* field,
	CudaField<T, C>* gradField,
	CudaField<double, F>* massFlowRateField,
	CudaLinearSolverMatrix<V>* matrix
);

// -------------------- Assemble Boundary Faces Implementation ----------------------

template<typename Obj, typename GradObj>
__device__ void ConvectionQUICK::assembleBoundariesImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<Obj, C>* field,
	CudaField<GradObj, C>* gradField,
	CudaField<double, F>* massFlowRateField,
	CudaLinearSolverMatrix<Obj>* matrix
) 
{
	ConvectionUpwind upwind{};

	upwind.assembleBoundaries(
		mesh,
		field,
		gradField,
		massFlowRateField,
		matrix
	);
}

template
__device__ void ConvectionQUICK::assembleBoundariesImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<double, C>* field,
	CudaField<V, C>* gradField,
	CudaField<double, F>* massFlowRateField,
	CudaLinearSolverMatrix<double>* matrix
);

template
__device__ void ConvectionQUICK::assembleBoundariesImpl(
	CudaMesh<MeshDim::D3>* mesh,
	CudaField<V, C>* field,
	CudaField<T, C>* gradField,
	CudaField<double, F>* massFlowRateField,
	CudaLinearSolverMatrix<V>* matrix
);

