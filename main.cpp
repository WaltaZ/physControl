#include <iostream>
#include <geometry/geometry.h>
#include <mesh/mesh.h>
#include <problem/problem.h>

int main() {
	Cuboid box = Cuboid(2, 4, 5);
	ProblemGeometry<GeometryDim::D3, Cuboid> problem({box});
	CartesianMesher<MeshDim::D3> mesher(problem);

	Mesh<MeshDim::D3> mesh = mesher.createMesh();
	

	//chuj(mesh);*/

	//geometryTests::runVolumeTest();

}