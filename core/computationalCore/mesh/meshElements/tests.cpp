#include "../include/mesh/meshElements/tests.h"

namespace meshElementsTests {
	void runFaceTest() {
		Face<MeshDim::D3> face(
			1.0,
			Vector<GeometryDim::D3>(),
			Point<GeometryDim::D3>(),
			std::vector<int>({ 0, 1, 6 }),
			1
		);

		Vector<GeometryDim::D3> normal = face.normalVector;
		geometryPrint::printV(normal);
		std::cout << "Is boundary: " << face.isBoundary() << std::endl;
	}
}