#include <simulation/heatTransfer/simulation.h>

HeatTransferSimulationD3::HeatTransferSimulationD3(
	HeatTransferProblemD3& problem,
	const Mesh<MeshDim::D3>* mesh,
	HeatTransferSimulationMethods&& methods
	) : problem(problem), mesh(mesh), methods(std::move(methods)) 
{
	problem.fields.velocity->initPastTrace(1);
	problem.fields.temperature->initPastTrace(1);
}

void HeatTransferSimulationD3::nextStep()
{
	
}