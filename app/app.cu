#include <app/app.h>

// ----------------------------------------- NEW PROBLEM --------------------------------------------

void AppSimulation::_newProblem()
{
	constexpr int amount = 50;

	Cuboid box = Cuboid(5, 4, 2);

	problem = new HeatTransferProblemD3(box);

	BoundaryConditionD3 test1(
		BoundaryConditionType::Drichlet,
		{ 300 },
		RectangleD3(
			new Point<GeometryDim::D3>({ 0, 0.2, 0 }),
			new Point<GeometryDim::D3>({ 0, 1.2, 0 }),
			new Point<GeometryDim::D3>({ 0, 1.2, 0.4 }),
			new Point<GeometryDim::D3>({ 0, 0.2, 0.4 })
		)
	);

	BoundaryConditionD3 test2(
		BoundaryConditionType::Drichlet,
		{ 288.0 },
		RectangleD3(
			new Point<GeometryDim::D3>({ 0, 0, 0 }),
			new Point<GeometryDim::D3>({ 5, 0, 0 }),
			new Point<GeometryDim::D3>({ 5, 0, 2 }),
			new Point<GeometryDim::D3>({ 0, 0, 2 })
		)
	);

	problem->addTemperatureBoundaryCondition(test1);
	problem->addTemperatureBoundaryCondition(test2);

	std::vector<double> division(amount + 1);

	for (int i = 0; i < amount + 1; i++) {
		division[i] = pow(((double)i / (double)amount), 3);
	}

	CartesianMesher<MeshDim::D3> mesher(problem, box, { amount, (amount), (amount) });

	mesh = mesher.generateMesh();

	mesh->save();
	problem->save();
};

// --------------------------------------- FILE PROBLEM ------------------------------------------

void AppSimulation::_fileProblem()
{
	mesh = Mesh<MeshDim::D3>::load();
	problem = new HeatTransferProblemD3();

	problem->load();
}

// ----------------------------------------- MAIN APP --------------------------------------------

void AppSimulation::runApp()
{
	_fileProblem();

	problem->initFields(mesh);

	fieldTests::setUpRadialField(
		problem->fields->temperature,
		mesh,
		problem->geometry.getCentroid(),
		293);

	fieldTests::setUpCurlyField(
		problem->fields->velocity,
		mesh,
		problem->geometry.getCentroid(),
		5.0,
		4.0);

	// ------------ Discretization terms -------------

	using V = Vector<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;

	HeatTransferSimulationD3 simulation(
		problem,
		mesh,
		HeatTransferSimulationMethods(
			cudaUtils::create<GradientGauss>(),
			cudaUtils::createInDevice<DiffusionSimple>(),
			cudaUtils::createInDevice<ConvectionUpwind>(),
			cudaUtils::createInDevice<UnsteadyEulerBackward>(),
			new LinearSolverJacobiFactory()
		)
	);

	FieldDisplayer displayer = FieldDisplayer(simulation);

	displayer.display();
}

AppSimulation::~AppSimulation()
{
	delete problem;
	cudaUtils::destroy(mesh);
}