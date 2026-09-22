#include <simulation/simulationCore/simulation.h>

#include <utility/debugUtils.h>
#include <utility/cudaUtilsWithKernels.h>

#include <simulation/discretization/unsteady/unsteadyEulerBackward.h>

HeatTransferSimulationD3::HeatTransferSimulationD3(
	HeatTransferProblemD3* problem,
	const Mesh<MeshDim::D3>* mesh,
	HeatTransferSimulationMethods&& methods) :
	data{
		problem,
		mesh,
		cudaUtils::create<HeatTransferSimulationMethods>(std::move(methods))
	},

	_simple(data)
{
	data.problem->fields->velocity->initPastTrace(1);
	data.problem->fields->temperature->initPastTrace(1);
	data.problem->fields->gradTemperature->initPastTrace(1);
	data.problem->fields->gradPressure->initPastTrace(1);
}

void HeatTransferSimulationD3::nextStep()
{
	_simple.nextStep();
	data.problem->fields->velocity->moveTraceToNextStep();
	data.problem->fields->temperature->moveTraceToNextStep();
	data.problem->fields->gradTemperature->moveTraceToNextStep();
	data.problem->fields->gradPressure->moveTraceToNextStep();
}