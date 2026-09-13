#include <simulation/simulationCore/simulationMethods.h>

#include <simulation/discretization/unsteady/unsteadyEulerBackward.h>

HeatTransferSimulationMethods::HeatTransferSimulationMethods(
	GradientBase* gradient,
	DiffusionBase* diffusion,
	ConvectionBase* convection,
	UnsteadyBase* unsteady,
	LinearSolverFactory* solverFactory

) : gradient(gradient),
diffusion(diffusion),
convection(convection),
unsteady(unsteady),
solverFactory(solverFactory)
{
	sourceGravity = cudaUtils::create<SourceGravity>();
	sourceBoussinesq = cudaUtils::create<SourceBoussinesq>();
};

HeatTransferSimulationMethods::~HeatTransferSimulationMethods()
{
	cudaUtils::destroy(gradient);
	cudaUtils::destroy(diffusion);
	cudaUtils::destroy(convection);
	cudaUtils::destroy(unsteady);
	cudaUtils::destroy(sourceGravity);
	cudaUtils::destroy(sourceBoussinesq);

	delete solverFactory;
};

HeatTransferSimulationMethods::HeatTransferSimulationMethods(
	HeatTransferSimulationMethods&& other)
	: gradient(other.gradient),
	diffusion(other.diffusion),
	convection(other.convection),
	unsteady(other.unsteady),
	solverFactory(other.solverFactory),
	sourceBoussinesq(other.sourceBoussinesq),
	sourceGravity(other.sourceGravity)
{
	other.gradient = nullptr;
	other.diffusion = nullptr;
	other.convection = nullptr;
	other.unsteady = nullptr;
	other.solverFactory = nullptr;
	other.sourceBoussinesq = nullptr;
	other.sourceGravity = nullptr;
};