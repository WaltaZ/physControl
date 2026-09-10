#include <simulation/linearSolver/jacobi/linearSolverJacobi.h>

#include <simulation/linearSolver/jacobi/linearSolverJacobiKernel.h>
#include <simulation/linearSolver/jacobi/linearSolverJacobiConfig.h>

#include <utility/cudaUtils.h>
#include <utility/mathUtils.h>

// ---------------------------- Linear Solver Jacobi ------------------------------

template<class Obj>
LinearSolverJacobi<Obj>::LinearSolverJacobi(
	const Mesh<MeshDim::D3>* mesh, 
	Field<Obj, Cell<MeshDim::D3>>* field) 
	: LinearSolver<Obj>(mesh, field) {};

template<class Obj>
void LinearSolverJacobi<Obj>::solve() {

	using namespace jacobiKernels;

	uint32_t fieldLength = _field->getLength();

	Obj* newPhi;
	cudaMalloc(&newPhi, fieldLength * sizeof(Obj));

	Obj* residuals;
	cudaMallocManaged(&residuals, fieldLength * sizeof(Obj));

	double* normResidual;
	cudaMallocManaged(&normResidual, sizeof(double));

	for (size_t i = 0; i < jacobiConfig::MAX_ITERATION; i++) {

		KernelArgs args = cudaUtils::getKernelArgs(_field->getLength());

		_nextStep << <args.blocks, args.threads >> > (newPhi, _field, matrix, _mesh);
		cudaUtils::fetchError(cudaDeviceSynchronize);

		_updateField << <args.blocks, args.threads >> > (newPhi, _field);
		_updateRisiduals << <args.blocks, args.threads >> > (
			residuals,
			newPhi,
			fieldLength,
			matrix,
			_mesh);
		cudaUtils::fetchError(cudaDeviceSynchronize);


		cudaMemset(normResidual, 0, sizeof(double));

		uint32_t numOfAdditions =
			fieldLength * mathUtils::getNumOfComp(_field->values[0]);

		args = cudaUtils::getKernelArgs(
			numOfAdditions,
			jacobiConfig::THREADS_PER_BLOCK);

		_addSquares << <args.blocks, args.threads >> > (
			residuals,
			normResidual,
			numOfAdditions);
		cudaUtils::fetchError(cudaDeviceSynchronize);

		*normResidual = sqrt(*normResidual);

		if (*normResidual < jacobiConfig::R_THRESHOLD) { break; }
	}

	cudaFree(newPhi);
	cudaFree(residuals);
	cudaFree(normResidual);
}

template class LinearSolverJacobi<double>;
template class LinearSolverJacobi<Vector<GeometryDim::D3>>;

// ------------------------- Linear Solver Jacobi Factory -----------------------------

std::unique_ptr<LinearSolver<double>> 
LinearSolverJacobiFactory::getSolver(
	const Mesh<MeshDim::D3>* mesh,
	Field<double, Cell<MeshDim::D3>>* field
) 
{
	return std::make_unique<LinearSolverJacobi<double>>
		(LinearSolverJacobi<double>(mesh, field));
};

std::unique_ptr<LinearSolver<Vector<GeometryDim::D3>>> 
LinearSolverJacobiFactory::getSolver(
	const Mesh<MeshDim::D3>* mesh,
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* field
) 
{
	return std::make_unique<LinearSolverJacobi<Vector<GeometryDim::D3>>>
		(LinearSolverJacobi<Vector<GeometryDim::D3>>(mesh, field));
};