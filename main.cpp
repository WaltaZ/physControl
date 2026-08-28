#include <iostream>
#include <geometry/geometry.h>
#include <mesh/mesh.h>
#include <problem/problem.h>
#include <visuals/displayer/displayer.h>

int main() {
	constexpr int amount = 20;

	Cuboid box = Cuboid(5, 4, 2);
	ProblemGeometryCuboid problemGeometry(box);

	HeatTransferD3 problem = HeatTransferD3(problemGeometry);
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
	
	/*BoundaryConditionD3 test2(
		BoundaryConditionType::Drichlet,
		{5.0},
		RectangleD3(
			new Point<GeometryDim::D3>({0.2, 4, 5}),
			new Point<GeometryDim::D3>({1.5, 4, 5}),
			new Point<GeometryDim::D3>({1.5, 4, 4.2}),
			new Point<GeometryDim::D3>({0.2, 4, 4.2})
		)
	);
	*/
	problem.addVelocityBoundaryCondition(test1);
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

	const auto& element = mesh.cells[mesh.cells.length-1];

	//size_t size = mesh.getMeshSize();

	//std::cout << size << " B" << "\n";
	//std::cout << (double)size/ 1048576.0 << " MB" << "\n";

	for (int i = 0; i < problem.fields.temperature.values.length; i++) {
		problem.fields.temperature.values[i] = pow(i, 1.1);
	}

	for (int i = 0; i < problem.fields.pressure.values.length; i++) {
		problem.fields.pressure.values[i] = sqrt(i);
	}

	for (int i = 0; i < problem.fields.velocity.values.length; i++) {
		problem.fields.velocity.values[i].comp[0] = pow(i, 1.1);
		problem.fields.velocity.values[i].comp[1] = pow(i, 1.1);
		problem.fields.velocity.values[i].comp[2] = pow(i, 1.1);
	}

	FieldDisplayer displayer(mesh, problem);

	displayer.display();
}