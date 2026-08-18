#include <iostream>
#include <geometry/geometry.h>
#include <mesh/mesh.h>
#include <problem/problem.h>
#include <vtkDisplay/vtkDisplay.h>

int main() {

	constexpr int amount = 20;

	Cuboid box = Cuboid(2, 4, 5);
	ProblemGeometry<GeometryDim::D3, Cuboid> problem({box});
	CartesianMesher<MeshDim::D3> mesher(problem, { amount, amount, amount });

	std::vector<double> division(amount + 1);

	for (int i = 0; i < amount + 1; i++) {
		division[i] = pow(((double)i/(double)amount), 5);
	}

	mesher.setDivisionPattern({ division, division }, 0);

	Mesh<MeshDim::D3> mesh = mesher.createMesh();
	
	displayTest(mesh);

}