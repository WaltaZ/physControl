#include <simulation/discretization/gradient/simpleGradient.h>
#include <geometry/vector.h>

void SimpleGradient::compute(
	Field<double, Cell<MeshDim::D3>>& field,
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
	Mesh<MeshDim::D3>& mesh
) {
	
	using namespace CUDA_SimpleGradient;

	// Calculating Gradient inside inner faces

	int blocks = ceil(
		(double)field.values.length / 
		(double)cudaConfig::defaultThreadsPerBlock);

	CUDA_compute_EC_internalFaces
		<< <blocks, cudaConfig::defaultThreadsPerBlock >> > (
			field.values.getData(),
			destField.values.getData(),
			mesh.getElements());

	cudaUtils::fetchError();
	cudaUtils::fetchError(cudaDeviceSynchronize);

	if (field.boundaryPatches.length == 0) {
		blocks = ceil(
			(double)mesh.getElements()->faces.length /
			(double)cudaConfig::defaultThreadsPerBlock);
		CUDA_compute_EF_noBC
			<< <blocks, cudaConfig::defaultThreadsPerBlock >> > (
				field.values.getData(),
				destField.values.getData(),
				mesh.getElements());
		cudaUtils::fetchError();
		cudaUtils::fetchError(cudaDeviceSynchronize);
	}

	for (int i = 0; i < field.boundaryPatches.length; i++) {
		blocks = ceil(
			(double)field.boundaryPatches[i].faceIDs.length /
			(double)cudaConfig::defaultThreadsPerBlock);

		for (int i = 0; i < field.boundaryPatches[i].faceIDs.length; i++) {
			std::cout << mesh.getElements()->faces[field.boundaryPatches[i].faceIDs[i]].ownerData.centroidToFace.magnitude << std::endl;
		}

		CUDA_compute_EF_BC
		<<<blocks, cudaConfig::defaultThreadsPerBlock>>>(
			field.values.getData(),
			destField.values.getData(),
			mesh.getElements(),
			&field.boundaryPatches[i]);

		cudaUtils::fetchError();
		cudaUtils::fetchError(cudaDeviceSynchronize);
	}
	
};

void SimpleGradient::compute(
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& field,
	Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
	Mesh<MeshDim::D3>& mesh
) {
};

// ================================================ KERNELS =========================================================

// ----------------------------------- CUDA compute Each Cell Internal Faces ------------------------------------

template<typename Obj, typename ObjDest>
__global__ 
void CUDA_SimpleGradient::CUDA_compute_EC_internalFaces(
	Obj* obj,
	ObjDest* destObj,
	CudaMesh<MeshDim::D3>* mesh)
{
	int id = blockIdx.x * blockDim.x + threadIdx.x;

	if (id >= mesh->cells.length) { return; }

	CudaArray<double> objArray = geomUtils::getComponents(&obj[id]);
	CudaArray<double> destObjArray = geomUtils::getComponents(&destObj[id]);

	assert(3 * objArray.length == destObjArray.length);

	for (int i = 0; i < objArray.length; i++) {
		Vector<GeometryDim::D3> grad = Vector<GeometryDim::D3>({ 0, 0, 0 });
		const auto& cell = mesh->cells[id];

		auto& value = objArray.getData()[i];

		for (int j = 0; j < cell.cellFaceIDs.length; j++) {
			const auto& face = mesh->faces[cell.cellFaceIDs[j]];

			if (!face.isBoundary) {

				CudaArray<double> neighbourObjArray = geomUtils::getComponents(&obj[face.getNeighbourCellID(id)]);

				double g_C = face.getWeightFactor(id);
				double g_F = 1 - g_C;

				double faceValue = (g_C * objArray[i]) + (g_F * neighbourObjArray[i]);
				grad = grad + (face.getArea(id).vector * faceValue);
			}
		}
		grad = grad / cell.volume;
		for (int j = 0; j < 3; j++) {
			destObjArray[i * objArray.length + j] = grad.comp[j];
		}
	}
}

template
__global__
void CUDA_SimpleGradient::CUDA_compute_EC_internalFaces(
	double* obj,
	Vector<GeometryDim::D3>* destObj,
	CudaMesh<MeshDim::D3>* mesh);

template
__global__
void CUDA_SimpleGradient::CUDA_compute_EC_internalFaces(
	Vector<GeometryDim::D3>* obj,
	MatrixTensor<GeometryDim::D3>* destObj,
	CudaMesh<MeshDim::D3>* mesh);

// ----------------------------------- CUDA compute Each Face No boundary conditions ------------------------------------
// TODO: Make templates out of it:

__global__ 
void CUDA_SimpleGradient::CUDA_compute_EF_noBC(
	double* value, 
	Vector<GeometryDim::D3>* destVector, 
	CudaMesh<MeshDim::D3>* mesh)
{
	int id = blockIdx.x * blockDim.x + threadIdx.x;

	if (id >= mesh->faces.length) { return; }
	
	const auto& face = mesh->faces[id];
	if (!face.isBoundary) { return; }

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

__global__ void CUDA_SimpleGradient::CUDA_compute_EF_BC(
	double* value, 
	Vector<GeometryDim::D3>* destVector,
	CudaMesh<MeshDim::D3>* mesh, 
	BoundaryPatch* bp)
{
	int id = blockIdx.x * blockDim.x + threadIdx.x;

	if (id >= bp->faceIDs.length) { return; }

	const auto& face = mesh->faces[bp->faceIDs[id]];
	uint32_t cellID = face.ownerCellID;
	const auto& cell = mesh->cells[cellID];
	const double* bpValues = bp->values.getData();

	Vector<GeometryDim::D3> contribution{};

	// Boundary condition handler
	switch (bp->type) {
	case BoundaryConditionType::Mixed:
		double gamma_over_d = (bpValues[2] / face.ownerData.centroidToFace.magnitude);
		double faceValue =
			(bpValues[0] * bpValues[1] + gamma_over_d * value[cellID]) /
			(bpValues[1] + gamma_over_d);
		printf("%lf\n", face.ownerData.centroidToFace.magnitude);

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

	for (int i = 0; i < 3; i++) {
		atomicAdd(&(destVector[cellID].comp[i]), contribution.comp[i]);
	}
}