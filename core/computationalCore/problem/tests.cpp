#include <problem/tests.h>

namespace fieldTests {
	void setUpRadialScalarField(
		Field<double, Cell<MeshDim::D3>>& field,
		const Mesh<MeshDim::D3>& mesh,
		Point<GeometryDim::D3> center)
	{
		std::cout << "Test\n";
		for (int i = 0; i < field.values.length; i++) {
			double r = Vector<GeometryDim::D3>(center, mesh.getElements()->cells[i].centroid).getMagnitude();
			field.values[i] = 1/pow(r, 2);
		}
	}
}