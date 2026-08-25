#include <iostream>
#include <geometry/geometry.h>
#include <mesh/mesh.h>
#include <problem/problem.h>
#include <vtkDisplay/vtkDisplay.h>

int main() {
	constexpr int amount = 20;

	Cuboid box = Cuboid(3, 3, 3);
	ProblemGeometryCuboid problemGeometry(box);

	HeatTransferD3 problem = HeatTransferD3(problemGeometry);
	BoundaryConditionD3 test1(
		BoundaryConditionType::Drichlet,
		{5.0},
		RectangleD3(
			new Point<GeometryDim::D3>({1, 0, 0}),
			new Point<GeometryDim::D3>({2, 0, 0}),
			new Point<GeometryDim::D3>({2, 1, 0}),
			new Point<GeometryDim::D3>({1, 1, 0})
		)
	);
	
	BoundaryConditionD3 test2(
		BoundaryConditionType::Drichlet,
		{5.0},
		RectangleD3(
			new Point<GeometryDim::D3>({0.2, 4, 5}),
			new Point<GeometryDim::D3>({1.5, 4, 5}),
			new Point<GeometryDim::D3>({1.5, 4, 4.2}),
			new Point<GeometryDim::D3>({0.2, 4, 4.2})
		)
	);
	
	problem.addVelocityBoundaryCondition(test1);
	//problem.addVelocityBoundaryCondition(test2);

	CartesianMesher<MeshDim::D3> mesher(problem, problemGeometry, { amount, amount, amount });

	std::vector<Point<GeometryDim::D3>*> test{};
	test.push_back(new Point<GeometryDim::D3>({1, 2, 3}));
	delete test[0];
	std::cout << test.size();

	std::vector<double> division(amount + 1);

	for (int i = 0; i < amount + 1; i++) {
		division[i] = pow(((double)i/(double)amount), 5);
	}

	//mesher.setDivisionPattern({ division, division }, 0);

	Mesh<MeshDim::D3> mesh = mesher.generateMesh();

	for (int i = 0; i < mesh.faces.data[2].faceNodeIDs.length; i++) {
		std::cout << mesh.elementsIDs.faceNodeIDs.data[i + mesh.faces.data[2].faceNodeIDs.offset] << ", ";
	}

	std::cout << '\n';

	//size_t size = mesh.getMeshSize();

	//std::cout << size << " B" << "\n";
	//std::cout << (double)size/ 1048576.0 << " MB" << "\n";

	displayTest(mesh);

}