#include <simulation/simulation.h>

#include <geometry/geometryUtils.h>
#include <simulation/discretization/discretization.h>
#include <simulation/linearSolver/linearSolverMatrix.h>

#include <utility/cudaUtilsWithKernels.h>

#include <utility/debugUtils.h>

HeatTransferSimulationD3::HeatTransferSimulationD3(
	HeatTransferProblemD3& problem,
	const Mesh<MeshDim::D3>* mesh) : problem(problem), mesh(mesh) {}

__global__
void testKernel(
	const Mesh<MeshDim::D3>* mesh,
	Field<double, Cell<MeshDim::D3>>* field,
	Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>* gradField,
	Field<double, Face<MeshDim::D3>>* massFlowRate,
	LinearSolverMatrix<double>* matrix,
	DiffusionBase* diffusion,
	ConvectionBase* convection,
	UnsteadyBase* unsteady
) 
{
	convection->assembleInner(mesh, field, gradField, massFlowRate, matrix);
	diffusion->assembleInner(mesh, field, matrix, 2e-5);
	unsteady->assemble(mesh, field, matrix, 0.001);
}

void HeatTransferSimulationD3::nextStep()
{
	// Update all the fields as a next step
	GradientGauss* gradient = cudaUtils::create<GradientGauss>();

	LinearSolverMatrix<double>* matrix = cudaUtils::create<LinearSolverMatrix<double>>(mesh);

	DiffusionBase* diffusion = cudaUtils::createInDevice<DiffusionSimple>();
	ConvectionBase* convection = cudaUtils::createInDevice<ConvectionUpwind>();
	UnsteadyBase* unsteady = cudaUtils::createInDevice<UnsteadyEulerBackward>();

	problem.fields.temperature->initPastTrace(2);

	KernelArgs args = cudaUtils::getKernelArgs(mesh->cells.length);
	testKernel<<<args.blocks, args.threads>>>(
		mesh,
		problem.fields.temperature,
		problem.fields.gradTemperature,
		problem.fields.massFlowRate,
		matrix,
		diffusion,
		convection,
		unsteady
	);

	cudaUtils::fetchError(cudaDeviceSynchronize);

	debug::printSolverMatrix(matrix);

	/*gradient->compute(
		problem.fields.temperature, 
		problem.fields.gradTemperature, 
		mesh);*/
}