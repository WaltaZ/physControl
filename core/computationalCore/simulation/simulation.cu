#include <simulation/simulation.h>

#include <geometry/geometryUtils.h>
#include <simulation/discretization/discretization.h>
#include <simulation/linearSolver/linearSolverMatrix.h>

#include <utility/debugUtils.h>

HeatTransferSimulationD3::HeatTransferSimulationD3(
	HeatTransferProblemD3& problem,
	const Mesh<MeshDim::D3>* mesh) : problem(problem), mesh(mesh) {}

__global__
void testKernel(
	Field<double, Cell<MeshDim::D3>>* field,
	double value) {
	
}

void HeatTransferSimulationD3::nextStep()
{
	// Update all the fields as a next step
	GradientGauss* gradient = cudaUtils::create<GradientGauss>();

	DiffusionBase* diffusion = cudaUtils::create<DiffusionSimple>();
	LinearSolverMatrix<double>* solver = cudaUtils::create<LinearSolverMatrix<double>>();

	problem.fields.temperature->initPastTrace(2);

	/*gradient->compute(
		problem.fields.temperature, 
		problem.fields.gradTemperature, 
		mesh);*/
}