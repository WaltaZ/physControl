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

	std::unique_ptr<LinearSolver<double>> pressureSolver = 
		methods->solverFactory->getSolver(mesh, fields.pressure);

	KernelArgs args_EC = cudaUtils::getKernelArgs(numOfCells);
	simKernel::assembleInnerVelocity <<<args_EC.blocks, args_EC.threads>>> (
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

	KernelArgs args = cudaUtils::getKernelArgs(fields.velocity->bpFaceIDsLengthMax);
	simKernel::assembleBoundariesVelocity<<<args.blocks, args.threads>>>(
		mesh,
		fields.velocity,
		fields.pressure,
		fields.gradPressure,
		velocitySolver->matrix
	);
	cudaUtils::fetchError(cudaDeviceSynchronize);

	velocitySolver->solve();

	KernelArgs args_EF = cudaUtils::getKernelArgs(numOfFaces);
	simKernel::updateMassFlow<<<args_EF.blocks, args_EF.threads>>>(
		mesh, 
		fields.velocity,
		fields.pressure,
		fields.gradPressure, 
		fields.massFlowRate, 
		velocitySolver->matrix
	);
	cudaUtils::fetchError(cudaDeviceSynchronize);

	simKernel::assemblePressure<<<args_EC.blocks, args_EC.threads>>>(
		mesh,
		fields.massFlowRate,
		pressureSolver->matrix
	);
	cudaUtils::fetchError(cudaDeviceSynchronize);

	pressureSolver->solve();

	debug::printSolverMatrix(pressureSolver->matrix);

 }