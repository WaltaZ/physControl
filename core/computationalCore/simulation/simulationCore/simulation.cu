#include <simulation/simulationCore/simulation.h>

#include <utility/debugUtils.h>
#include <utility/cudaUtilsWithKernels.h>

#include <simulation/discretization/unsteady/unsteadyEulerBackward.h>

HeatTransferSimulationD3::HeatTransferSimulationD3(
	HeatTransferProblemD3& problem,
	const Mesh<MeshDim::D3>* mesh,
	HeatTransferSimulationMethods&& methods) :
	data(
		problem,
		mesh,
		cudaUtils::create<HeatTransferSimulationMethods>(std::move(methods))
	),

	_simple(data)
{
	problem.fields->velocity->initPastTrace(1);

	problem.fields->gradPressure->initPastTrace(1);

	problem.fields->temperature->initPastTrace(1);
}

void HeatTransferSimulationD3::nextStep()
{
	_simple.nextStep();
}