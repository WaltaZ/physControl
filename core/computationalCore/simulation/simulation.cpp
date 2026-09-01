#include <simulation/simulation.h>

#include <geometry/geometryUtils.h>

HeatTransferSimulationD3::HeatTransferSimulationD3(
	HeatTransferProblemD3& problem,
	Mesh<MeshDim::D3>& mesh) : _problem(problem), _mesh(mesh) {}

HeatTransferProblemD3& HeatTransferSimulationD3::getProblem()
{
	return _problem;
}

Mesh<MeshDim::D3>& HeatTransferSimulationD3::getMesh()
{
	return _mesh;
}

void HeatTransferSimulationD3::nextStep()
{
	// Update all the fields as a next step
	auto gradient = SimpleGradient();

	gradient.compute(
		_problem.fields.temperature, 
		_problem.fields.gradTemperature, 
		_mesh);

	/*for (int i = 0; i < _problem.fields.gradTemperature.values.length; i++) {
		geomPrint::printV(_problem.fields.gradTemperature.values[i]);
	}*/
}
