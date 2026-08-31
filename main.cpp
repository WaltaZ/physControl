#include <iostream>
#include <geometry/geometry.h>
#include <mesh/mesh.h>
#include <problem/problem.h>
#include <problem/tests.h>
#include <visuals/displayer/displayer.h>

int main() {
	constexpr int amount = 20;

	Cuboid box = Cuboid(5, 4, 2);
	ProblemGeometryCuboid problemGeometry(box);

	HeatTransferProblemD3 problem = HeatTransferProblemD3(problemGeometry);
	BoundaryConditionD3 test1(
		BoundaryConditionType::Drichlet,
		{5.0},
		RectangleD3(
			new Point<GeometryDim::D3>({0, 0.2, 0}),
			new Point<GeometryDim::D3>({0, 1.2, 0}),
			new Point<GeometryDim::D3>({0, 1.2, 0.4}),
			new Point<GeometryDim::D3>({0, 0.2, 0.4})
		)
	);
	
	BoundaryConditionD3 test2(
		BoundaryConditionType::Drichlet,
		{5.0},
		RectangleD3(
			new Point<GeometryDim::D3>({ 5, 0.2, 0 }),
			new Point<GeometryDim::D3>({ 5, 1.2, 0 }),
			new Point<GeometryDim::D3>({ 5, 1.2, 0.4 }),
			new Point<GeometryDim::D3>({ 5, 0.2, 0.4 })
		)
	);
	
	//problem.addVelocityBoundaryCondition(test1);
	//problem.addVelocityBoundaryCondition(test2);

	std::vector<double> division(amount + 1);

	for (int i = 0; i < amount + 1; i++) {
		division[i] = pow(((double)i/(double)amount), 3);
	}

	CartesianMesher<MeshDim::D3> mesher(problem, problemGeometry, { amount, (int)(amount*0.8), (int)(amount*0.4)});

	//mesher.setDivisionPattern(division, 0);

	//mesher.setDivisionPattern({ division, division }, 0);

	Mesh<MeshDim::D3> mesh = mesher.generateMesh();
	problem.initFields(mesh);

	fieldTests::setUpRadialScalarField(problem.fields.pressure, mesh, box.getCentroid());

	HeatTransferSimulationD3 simulation(problem, mesh);

	FieldDisplayer displayer = FieldDisplayer(simulation);

	displayer.display();
}