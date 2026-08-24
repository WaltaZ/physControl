#include <iostream>
#include <geometry/geometry.h>
#include <mesh/mesh.h>
#include <problem/problem.h>
#include <vtkDisplay/vtkDisplay.h>

int main() {
	constexpr int amount = 10;

	Cuboid box = Cuboid(3, 4, 5);
	ProblemGeometryCuboid problemGeometry(box);

	RoomHeatTransferD3 problem = RoomHeatTransferD3(problemGeometry);
	
	BoundaryConditionD3 < Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>> test1{
		&problem.velocity,
		BoundaryConditionType::Drichlet,
		{5.0},
		RectangleD3(
			new Point<GeometryDim::D3>({0.5, 0, 0}),
			new Point<GeometryDim::D3>({1.75, 0, 0}),
			new Point<GeometryDim::D3>({1.75, 0.5, 0}),
			new Point<GeometryDim::D3>({0, 0.5, 0})
		)
	};

	BoundaryConditionD3 < Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>> test2{
		&problem.velocity,
		BoundaryConditionType::Drichlet,
		{5.0},
		RectangleD3(
			new Point<GeometryDim::D3>({0.2, 4, 5}),
			new Point<GeometryDim::D3>({1.5, 4, 5}),
			new Point<GeometryDim::D3>({1.5, 4, 4.2}),
			new Point<GeometryDim::D3>({0.2, 4, 4.2})
		)
	};
	
	problem.addVelocityBoundaryCondition(test1);
	problem.addVelocityBoundaryCondition(test2);

	CartesianMesher<MeshDim::D3> mesher(problem, problemGeometry, { amount, amount, amount });

	std::vector<double> division(amount + 1);

	for (int i = 0; i < amount + 1; i++) {
		division[i] = pow(((double)i/(double)amount), 5);
	}

	//mesher.setDivisionPattern({ division, division }, 0);

	Mesh<MeshDim::D3> mesh = mesher.createMesh();

	for (int i = 0; i < mesh.faces[2].faceNodeIDs.length; i++) {
		std::cout << mesh.elementsIDs.faceNodeIDs[i + mesh.faces[2].faceNodeIDs.offset] << ", ";
	}

	std::cout << '\n';

	//size_t size = mesh.getMeshSize();

	//std::cout << size << " B" << "\n";
	//std::cout << (double)size/ 1048576.0 << " MB" << "\n";

	displayTest(mesh);

}