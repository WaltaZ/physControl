#include <simulation/linearSolver/linearSolverMatrix.h>

CudaLinearSolverMatrix::CudaLinearSolverMatrix(
	const Mesh<MeshDim::D3>& mesh
) 
{
	// TODO: This is only for the nearest neighbourhood. If I ever 
	// wanted to implement something more complex, this needs to be changed \/

	const uint32_t numOfCells = 
		mesh.getElements()->cells.length;

	const uint32_t numOfNeighbourCells = 
		mesh.getElements()->
		elementsIDs.cellNeighbourCells.length;

	// A_C
	cudaMallocManaged(
		A_C.getDataPointer(), 
		numOfCells * sizeof(double));
	cudaMemset(A_C.getData(), 0, numOfCells);
	A_C.length = numOfCells;

	// A_Fs
	cudaMallocManaged(
		A_Fs.getDataPointer(),
		numOfNeighbourCells * sizeof(double)
	);
	cudaMemset(A_C.getData(), 0, numOfNeighbourCells);
	A_Fs.length = numOfNeighbourCells;

	// A_F
	cudaMallocManaged(
		A_F.getDataPointer(),
		numOfCells * sizeof(CudaArray<double>)
	);
	A_F.length = numOfCells;

	uint32_t offset = 0;
	for (int i = 0; i < numOfCells; i++) {
		uint32_t length = 
			mesh.getElements()->
			cells[i].cellNeighbourCells.length;

		A_F[i] = CudaArray<double>(
			A_Fs.getData(),
			offset,
			length
		);
	}

	// B
	cudaMallocManaged(
		B.getDataPointer(),
		numOfCells * sizeof(double)
	);
	cudaMemset(B.getData(), 0, numOfCells);
	B.length = numOfCells;
}

LinearSolverMatrix::LinearSolverMatrix(
	const Mesh<MeshDim::D3>& mesh) 
{
	cudaMallocManaged(
		&_linearSolverMatrix,
		sizeof(CudaLinearSolverMatrix));

	new(_linearSolverMatrix) CudaLinearSolverMatrix(mesh);
}

CudaLinearSolverMatrix* LinearSolverMatrix::getElements() {
	return _linearSolverMatrix;
}

const CudaLinearSolverMatrix* LinearSolverMatrix::getElements() const {
	return _linearSolverMatrix;
}