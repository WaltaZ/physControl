#include <simulation/discretization/gradient/gradientGaussKernel.h>

// ----------------------------------- CUDA compute Each Cell Internal Faces ------------------------------------

namespace CUDA_GradientGauss {

	template<typename Obj, typename ObjDest>
	__global__
		void CUDA_compute_EC_internalFaces(
			CudaField<Obj, Cell<MeshDim::D3>>* field,
			CudaField<ObjDest, Cell<MeshDim::D3>>* destField,
			CudaMesh<MeshDim::D3>* mesh)
	{
		int id = blockIdx.x * blockDim.x + threadIdx.x;

		if (id >= mesh->cells.length) { return; };

		Obj* obj = field->values.getData();
		ObjDest* destObj = destField->values.getData();

		CudaArray<double> objArray = geomUtils::getComponents(&(obj[id]));
		CudaArray<double> destObjArray = geomUtils::getComponents(&(destObj[id]));

		assert(3 * objArray.length == destObjArray.length);

		for (int i = 0; i < objArray.length; i++) {
			Vector<GeometryDim::D3> grad = Vector<GeometryDim::D3>({ 0, 0, 0 });
			const auto& cell = mesh->cells[id];

			auto& value = objArray.getData()[i];

			for (int j = 0; j < cell.cellFaceIDs.length; j++) {
				Vector<GeometryDim::D3> gradContribution = Vector<GeometryDim::D3>({ 0, 0, 0 });

				const auto& face = mesh->faces[cell.cellFaceIDs[j]];

				if (face.isBoundary) { continue; }

				CudaArray<double> neighbourObjArray =
					geomUtils::getComponents(&obj[face.getNeighbourCellID(id)]);

				double g_C = face.getWeightFactor(id);
				double g_F = 1 - g_C;

				double faceValue = (g_C * objArray[i]) + (g_F * neighbourObjArray[i]);

				//printf("Face nr: %d, Center value: %lf, Face value: %lf\n", cell.cellFaceIDs[i], value, faceValue);
				gradContribution = face.getArea(id).vector * faceValue;
				grad = grad + gradContribution;
				//printf("Cell nr. %d | Face value: %lf | Gradient value: [ %lf, %lf, %lf ]\n", id, faceValue, gradContribution.comp[0], gradContribution.comp[1], gradContribution.comp[2]);
			}
			grad = grad / cell.volume;
			for (int j = 0; j < 3; j++) {
				destObjArray[i * objArray.length + j] = grad.comp[j];
			}
		}
	}

	template
		__global__
		void CUDA_GradientGauss::CUDA_compute_EC_internalFaces(
			CudaField<double, Cell<MeshDim::D3>>* field,
			CudaField<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* destField,
			CudaMesh<MeshDim::D3>* mesh);

	template
		__global__
		void CUDA_GradientGauss::CUDA_compute_EC_internalFaces(
			CudaField<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* field,
			CudaField<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>* destField,
			CudaMesh<MeshDim::D3>* mesh);

	// ----------------------------------- CUDA compute Each Face No boundary conditions ------------------------------------
	// TODO: Make templates out of it:

	__global__
		void CUDA_compute_EF_noBC(
			CudaField<double, Cell<MeshDim::D3>>* field,
			CudaField<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* destField,
			CudaMesh<MeshDim::D3>* mesh)
	{
		int id = blockIdx.x * blockDim.x + threadIdx.x;

		if (id >= mesh->faces.length) { return; }

		const auto& face = mesh->faces[id];
		if (!face.isBoundary) { return; }

		double* value = field->values.getData();
		Vector<GeometryDim::D3>* destVector = destField->values.getData();

		uint32_t cellID = face.ownerCellID;
		const auto& cell = mesh->cells[cellID];

		double cellValue = value[cellID];
		Vector<GeometryDim::D3> contribution =
			face.getArea(cellID).vector *
			(cellValue / cell.volume);

		for (int i = 0; i < 3; i++) {
			atomicAdd(&(destVector[cellID].comp[i]), contribution.comp[i]);
		}
	}

	// ----------------------------------- CUDA compute Each Face boundary conditions ------------------------------------

	__global__ void CUDA_compute_EF_BC(
		CudaField<double, Cell<MeshDim::D3>>* field,
		CudaField<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* destField,
		CudaMesh<MeshDim::D3>* mesh)
	{
		int id = blockIdx.x * blockDim.x + threadIdx.x;

		const auto& boundaryPatches = field->boundaryPatches;

		for (size_t i = 0; i < boundaryPatches.length; i++)
		{
			auto& bp = boundaryPatches[i];

			if (id >= bp.faceIDs.length) { continue; }

			double* value = field->values.getData();
			Vector<GeometryDim::D3>* destVector = destField->values.getData();

			const uint32_t faceID = bp.faceIDs[id];
			const auto& face = mesh->faces[faceID];
			uint32_t cellID = face.ownerCellID;
			const auto& cell = mesh->cells[cellID];
			const double* bpValues = bp.values.getData();

			Vector<GeometryDim::D3> contribution{};

			// Boundary condition handler
			switch (bp.type) {
			case BoundaryConditionType::Mixed:
				double gamma_over_d = (bpValues[2] / face.ownerData.centroidToFace.magnitude);
				double faceValue =
					(bpValues[0] * bpValues[1] + gamma_over_d * value[cellID]) /
					(bpValues[1] + gamma_over_d);

				contribution =
					face.getArea(cellID).vector *
					(faceValue / cell.volume);
				break;

			case BoundaryConditionType::Drichlet:
				contribution =
					face.getArea(cellID).vector *
					(bpValues[0] / cell.volume);
				break;

			case BoundaryConditionType::Neumann:
				contribution =
					face.getArea(cellID).normal *
					bpValues[0];
				break;
			}

			for (int j = 0; j < 3; j++) {
				atomicAdd(&(destVector[cellID].comp[j]), contribution.comp[j]);
			}

		}
	}
}