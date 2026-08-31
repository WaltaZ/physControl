#include <simulation/discretization/gradient/simpleGradient.h>
#include <geometry/vector.h>

void SimpleGradient::compute(
	Field<double, Cell<MeshDim::D3>>& field,
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
	Mesh<MeshDim::D3>& mesh
) {

	int blocks = ceil((double)field.values.length / (double)cudaConfig::defaultThreadsPerBlock);

	SimpleGradientCuda::_compute_EC_internalFaces
		<< <blocks, cudaConfig::defaultThreadsPerBlock >> > (
			field.values.getData(),
			destField.values.getData(),
			mesh.getElements());

	cudaError_t err = cudaGetLastError();

	if (err != cudaSuccess) {
		printf("Kernel launch error: %s\n", cudaGetErrorString(err));
	}

	err = cudaDeviceSynchronize();

	if (err != cudaSuccess) {
		printf("Kernel execution error: %s\n", cudaGetErrorString(err));
	}
};

void SimpleGradient::compute(
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& field,
	Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
	Mesh<MeshDim::D3>& mesh
) {
};

__global__ void SimpleGradientCuda::_compute_EC_internalFaces(
	double* values,
	Vector<GeometryDim::D3>* destVector,
	CudaMesh<MeshDim::D3>* mesh)
{
	int id = blockIdx.x * blockDim.x + threadIdx.x;

	if (id < mesh->cells.length) {
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
};