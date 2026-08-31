#include <simulation/discretization/gradient/simpleGradient.h>
#include <geometry/vector.h>

void SimpleGradient::compute(
	Field<double, Cell<MeshDim::D3>>& field,
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
	Mesh<MeshDim::D3>& mesh
) {
	
	using namespace CUDA_SimpleGradient;

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

	blocks = ceil(
		(double)mesh.getElements()->faces.length / 
		(double)cudaConfig::defaultThreadsPerBlock);

	/*for (int id = 0; id < mesh.getElements()->faces.length; id++) {

		const auto& face = mesh.getElements()->faces[id];
		if (face.isBoundary) {
			uint32_t cellID = face.ownerCellID;
			const auto& cell = mesh.getElements()->cells[cellID];

			double cellValue = field.values[cellID];
			destField.values[cellID] =
				destField.values[cellID] +
				(face.getArea(cellID).vector * (cellValue / cell.volume));
		}
	}*/

	CUDA_compute_EF_noBC 
		<<<blocks, cudaConfig::defaultThreadsPerBlock >>> (
			field.values.getData(),
			destField.values.getData(),
			mesh.getElements());

	cudaUtils::fetchError();
	cudaUtils::fetchError(cudaDeviceSynchronize);
	
};

void SimpleGradient::compute(
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& field,
	Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
	Mesh<MeshDim::D3>& mesh
) {
};

// ------------------------------- KERNELS -----------------------------------

__global__ 
void CUDA_SimpleGradient::CUDA_compute_EC_internalFaces(
	double* values,
	Vector<GeometryDim::D3>* destVector,
	CudaMesh<MeshDim::D3>* mesh)
{
	int id = blockIdx.x * blockDim.x + threadIdx.x;

	if (id >= mesh->cells.length) { return; }

	Vector<GeometryDim::D3> grad = Vector<GeometryDim::D3>({ 0, 0, 0 });
	const auto& cell = mesh->cells[id];

	for (int i = 0; i < cell.cellFaceIDs.length; i++) {
		const auto& face = mesh->faces[cell.cellFaceIDs[i]];

		if (!face.isBoundary) {

			double g_C = face.getWeightFactor(id);
			double g_F = 1 - g_C;

			double faceValue = (g_C * values[id]) + (g_F * values[face.getNeighbourCellID(id)]);
			grad = grad + (face.getArea(id).vector * faceValue);
		}
	}
	destVector[id] = grad / cell.volume;
}

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
};

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
	const auto& bpValues = bp->values;

	Vector<GeometryDim::D3> contribution{};

	// Boundary condition handler
	switch (bp->type) {
	case BoundaryConditionType::Mixed:
		double gamma_over_d = (bpValues[2] / face.ownerData.centroidToFace.magnitude);
		double cellValue =
			(bpValues[0] * bpValues[1] + gamma_over_d * value[cellID]) /
			(bpValues[1 + gamma_over_d]);
		contribution = 
			face.getArea(cellID).vector *
			(cellValue / cell.volume);

	case BoundaryConditionType::Drichlet:
		contribution = 
			face.getArea(cellID).vector *
			(bpValues[0] / cell.volume);

	case BoundaryConditionType::Neumann:
		contribution =
			face.getArea(cellID).normal * 
			bpValues[0];
	}
}