#include <iostream>
#include <geometry.h>
#include <mesh.h>
#include <vtkDisplay.h>

struct Test {
	int a;
	Test(const int& a) : a(a) {};
};

int main() {
	Cuboid box = Cuboid(2, 4, 5);
	ProblemGeometry<GeometryDim::D3, Cuboid> problem({box});
	CartesianMesher<MeshDim::D3> mesher(problem);

	Mesh<MeshDim::D3> mesh = mesher.createMesh();
	

	chuj(mesh);

}