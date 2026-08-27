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

	mesher.setDivisionPattern(division, 0);

	//mesher.setDivisionPattern({ division, division }, 0);

	Mesh<MeshDim::D3> mesh = mesher.generateMesh();
	problem.initFields(mesh);

	const auto& element = mesh.cells[mesh.cells.length-1];

	for (size_t i = 0; i < element.cellNodeIDs.length; i++) {
		std::cout << "( ";
		for (int j = 0; j < 3; j++) {
			std::cout << mesh.nodes[element.cellNodeIDs[i]].pos[j];
			if(j!=2) {
				std::cout << ", ";
			}
		}
		std::cout << " )\n";
	}

	//size_t size = mesh.getMeshSize();

	//std::cout << size << " B" << "\n";
	//std::cout << (double)size/ 1048576.0 << " MB" << "\n";

	for (int i = 0; i < problem.fields.temperature.values.length; i++) {
		problem.fields.temperature.values[i] = rand() % 100;
		std::cout << problem.fields.temperature.values[i] << ", ";
	}

	FieldDisplayer displayer(mesh, problem);

	displayer.display();
}