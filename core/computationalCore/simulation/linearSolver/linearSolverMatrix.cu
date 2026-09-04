#include <simulation/linearSolver/linearSolverMatrix.h>

// --------------------- Cuda Linear Solver Matrix ---------------------------

template<typename Obj>
CudaLinearSolverMatrix<Obj>::CudaLinearSolverMatrix(
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
		numOfCells * sizeof(Obj)
	);
	for (size_t i = 0; i < numOfCells; i++)
	{
		B[i] = Obj();
	}
	B.length = numOfCells;
}

template<typename Obj>
__device__
void CudaLinearSolverMatrix<Obj>::contributeTo_A_C(
	uint32_t cellID,
	double A_C_contribution) 
{
	atomicAdd(&(A_C[cellID]), A_C_contribution);
};

template<typename Obj>
__device__
void CudaLinearSolverMatrix<Obj>::contributeTo_B(
	uint32_t cellID,
	double* B_contribution) 
{
	printf("CudaLinearSolverMatrix: Illegal object!");
};

template<>
__device__
void CudaLinearSolverMatrix<double>::contributeTo_B(
	uint32_t cellID,
	double* B_contribution) 
{
	atomicAdd(&B[cellID], *B_contribution);
};

template<>
__device__
void CudaLinearSolverMatrix<Vector<GeometryDim::D3>>::contributeTo_B(
	uint32_t cellID,
	double* B_contribution) 
{
	for (size_t comp = 0; comp < 3; comp++)
	{
		atomicAdd(&(B[cellID].comp[comp]), B_contribution[comp]);
	}
};

template class CudaLinearSolverMatrix<double>;
template class CudaLinearSolverMatrix<Vector<GeometryDim::D3>>;


// ------------------------ Linear Solver Matrix ---------------------------

template<typename Obj>
LinearSolverMatrix<Obj>::LinearSolverMatrix(
	const Mesh<MeshDim::D3>& mesh) 
{
	cudaMallocManaged(
		&_linearSolverMatrix,
		sizeof(CudaLinearSolverMatrix<Obj>));

	new(_linearSolverMatrix) CudaLinearSolverMatrix<Obj>(mesh);
}

template<typename Obj>
CudaLinearSolverMatrix<Obj>* LinearSolverMatrix<Obj>::getElements() {
	return _linearSolverMatrix;
}

template<typename Obj>
const CudaLinearSolverMatrix<Obj>* LinearSolverMatrix<Obj>::getElements() const {
	return _linearSolverMatrix;
}

template class LinearSolverMatrix<double>;
template class LinearSolverMatrix<Vector<GeometryDim::D3>>;