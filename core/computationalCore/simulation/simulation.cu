#include <simulation/simulation.h>

#include <geometry/geometryUtils.h>
#include <simulation/discretization/discretization.h>

#include <utility/debugUtils.h>

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

__global__
void testKernel(CudaField<double, Cell<MeshDim::D3>>* field, double value) {
	int id = blockDim.x * blockIdx.x + threadIdx.x;

	if (id >= field->values.length) { return; }

	field->moveTraceToNextStep();
	field->values[id] = value;
}

void HeatTransferSimulationD3::nextStep()
{
	// Update all the fields as a next step
	GradientGauss* gradient = cudaUtils::create<GradientGauss>();

	auto diffusion = DiffusionSimple();

	auto convection = ConvectionQUICK();

	auto unsteady = UnsteadyEulerBackward();

	_problem.fields.temperature.getElements()->initPastTrace(2);

	gradient->compute(
		_problem.fields.temperature, 
		_problem.fields.gradTemperature, 
		_mesh);
}