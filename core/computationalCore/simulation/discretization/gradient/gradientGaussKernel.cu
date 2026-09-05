#include <simulation/discretization/gradient/gradientGaussKernel.h>

// ----------------------------------- CUDA compute Each Cell Internal Faces ------------------------------------

namespace CUDA_GradientGauss {

	template<typename Obj, typename GradObj>
	__global__ void CUDA_compute_EC_internalFaces(
		CudaField<Obj, C>* field,
		CudaField<GradObj, C>* gradField,
		CudaMesh<MeshDim::D3>* mesh)
	{
		int C_id = blockIdx.x * blockDim.x + threadIdx.x;

		if (C_id >= mesh->cells.length) { return; };
		
		GradObj grad{};
		const auto& C = mesh->cells[C_id];

		const Obj& phi_C = field->values[C_id];

		for (int j = 0; j < C.cellFaceIDs.length; j++) {

			const auto& face = mesh->faces[C.cellFaceIDs[j]];

			if (face.isBoundary) { continue; }

			const uint32_t F_id = face.getNeighbourCellID(C_id);

			const Obj& phi_F = field->values[F_id];

			double g_C = face.getWeightFactor(C_id);
			double g_F = 1 - g_C;

			Obj faceValue = (phi_C * g_C) + (phi_F * g_F);

			grad = grad + (faceValue * face.getArea(C_id).vector);
		}
		grad = grad / C.volume;
		
		gradField->values[C_id] = grad;
	}

	template
	__global__ void CUDA_GradientGauss::CUDA_compute_EC_internalFaces(
		CudaField<double, C>* field,
		CudaField<V, C>* gradField,
		CudaMesh<MeshDim::D3>* mesh);

	template
	__global__ void CUDA_GradientGauss::CUDA_compute_EC_internalFaces(
		CudaField<V, C>* field,
		CudaField<T, C>* gradField,
		CudaMesh<MeshDim::D3>* mesh);

	// ----------------------------------- CUDA compute Each Face No boundary conditions ------------------------------------

	template<typename Obj, typename GradObj>
	__global__ void CUDA_compute_EF_noBC(
		CudaField<Obj, C>* field,
		CudaField<GradObj, C>* gradField,
		CudaMesh<MeshDim::D3>* mesh)
	{
		int f_id = blockIdx.x * blockDim.x + threadIdx.x;

		if (f_id >= mesh->faces.length) { return; }

		const auto& face = mesh->faces[f_id];
		if (!face.isBoundary) { return; }

		const uint32_t C_id = face.ownerCellID;
		const auto& C = mesh->cells[C_id];

		const Obj& phi_C = field->values[C_id];

		GradObj contribution =
			(phi_C * face.getArea(C_id).vector) / C.volume;
		
		cudaUtils::contributeTo(gradField->values[C_id], contribution);
	}

	template
	__global__ void CUDA_compute_EF_noBC(
		CudaField<double, C>* field,
		CudaField<V, C>* gradField,
		CudaMesh<MeshDim::D3>* mesh);

	template
	__global__ void CUDA_compute_EF_noBC(
		CudaField<V, C>* field,
		CudaField<T, C>* gradField,
		CudaMesh<MeshDim::D3>* mesh);

	// ----------------------------------- CUDA compute Each Face boundary conditions ------------------------------------

	// TODO: Make it more clear by splitting it into functions, like _handleBoundaryPatches();
	template<typename Obj, typename GradObj>
	__global__ void CUDA_compute_EF_BC(
		CudaField<Obj, C>* field,
		CudaField<GradObj, C>* gradField,
		CudaMesh<MeshDim::D3>* mesh)
	{
		int bp_faceId = blockIdx.x * blockDim.x + threadIdx.x;

		const auto& boundaryPatches = field->boundaryPatches;

		for (size_t patchNum = 0; patchNum < boundaryPatches.length; patchNum++)
		{
			auto& bp = boundaryPatches[patchNum];

			if (bp_faceId >= bp.faceIDs.length) { continue; }

			const uint32_t f_id = bp.faceIDs[bp_faceId];
			const auto& f = mesh->faces[f_id];
			uint32_t C_id = f.ownerCellID;
			const auto& C = mesh->cells[C_id];
			const double* bp_values = bp.values.getData();

			const Obj& phi_C = field->values[C_id];

			Obj phi_b = mathUtils::createMathObj<Obj>(bp_values);
			uint32_t phi_b_numOfComp = mathUtils::getNumOfComp(phi_b);
			GradObj contribution{};

			// Boundary condition handler
			switch (bp.type) {
			case BoundaryConditionType::Mixed:

				double h_infty = bp_values[phi_b_numOfComp];
				double gamma_b = bp_values[phi_b_numOfComp + 1];

				double gamma_over_d = 
				(
					gamma_b
					/ f.ownerData.centroidToFace.magnitude
				);
				Obj faceValue =
					(
						(
							h_infty * phi_b
						)
						+
						(
							phi_C * gamma_over_d
						)
					)
					/
					(
						h_infty + gamma_over_d
					);

				contribution =
					(faceValue / C.volume)
					* f.getArea(C_id).vector;
				break;

			case BoundaryConditionType::Drichlet:
				contribution =
					(phi_b / C.volume)
					* f.getArea(C_id).vector;
				break;

			case BoundaryConditionType::Neumann:
				contribution =
					(phi_b / C.volume)
					* f.getArea(C_id).normal;
				break;
			}
			cudaUtils::contributeTo<GradObj>(gradField->values[C_id], contribution);
		}
	}

	template
	__global__ void CUDA_compute_EF_BC(
		CudaField<double, C>* field,
		CudaField<V, C>* gradField,
		CudaMesh<MeshDim::D3>* mesh);

	template
	__global__ void CUDA_compute_EF_BC(
		CudaField<V, C>* field,
		CudaField<T, C>* gradField,
		CudaMesh<MeshDim::D3>* mesh);

}