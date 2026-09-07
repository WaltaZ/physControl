#include <simulation/linearSolver/linearSolverMatrix.h>

// --------------------- Cuda Linear Solver Matrix ---------------------------

template<typename Obj>
LinearSolverMatrix<Obj>::LinearSolverMatrix(
	const Mesh<MeshDim::D3>* mesh
) 
{
	// TODO: This is only for the nearest neighbourhood. If I ever 
	// wanted to implement something more complex, this needs to be changed \/

	const uint32_t numOfCells = 
		mesh->cells.length;

	const uint32_t numOfNeighbourCells = 
		mesh->
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
			mesh->
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
		numOfCells * sizeof(Obj)
	);
	for (size_t i = 0; i < numOfCells; i++)
	{
		B[i] = Obj();
	}
	B.length = numOfCells;
}

template class LinearSolverMatrix<double>;
template class LinearSolverMatrix<Vector<GeometryDim::D3>>;