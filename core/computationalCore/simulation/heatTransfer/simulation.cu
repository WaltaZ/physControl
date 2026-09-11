#include <simulation/heatTransfer/simulation.h>
#include <simulation/heatTransfer/simulationKernel.h>

#include <utility/debugUtils.h>
#include <utility/cudaUtilsWithKernels.h>

#include <simulation/discretization/unsteady/unsteadyEulerBackward.h>

HeatTransferSimulationD3::HeatTransferSimulationD3(
	HeatTransferProblemD3& problem,
	const Mesh<MeshDim::D3>* mesh,
	HeatTransferSimulationMethods&& methods
	) : problem(problem), mesh(mesh)
{
	problem.fields.velocity->initPastTrace(1);
	problem.fields.temperature->initPastTrace(1);

	this->methods = 
		cudaUtils::create<HeatTransferSimulationMethods>(std::move(methods));
}

void HeatTransferSimulationD3::nextStep()
{

	uint32_t numOfCells = mesh->cells.length;
	uint32_t numOfFaces = mesh->faces.length;

	auto& fields = problem.fields;

	fields.velocity->moveTraceToNextStep();
	fields.temperature->updateMaxMin();


	methods->gradient->compute(
		fields.pressure,
		fields.gradPressure,
		mesh
	);

	std::unique_ptr<LinearSolver<Vector<GeometryDim::D3>>> velocitySolver = 
		methods->solverFactory->getSolver(mesh, fields.velocity);

	KernelArgs args = cudaUtils::getKernelArgs(numOfCells);
	simKernel::assembleInnerVelocity <<<args.blocks, args.threads>>> (
		mesh,
		fields.velocity,
		fields.gradVelocity,
		fields.temperature,
		fields.massFlowRate,
		fields.gradPressure,
		velocitySolver->matrix,
		methods
	);
	cudaUtils::fetchError(cudaDeviceSynchronize);

	velocitySolver->solve();

	debug::printSolverMatrix(velocitySolver->matrix);
}