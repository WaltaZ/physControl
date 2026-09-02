#include <problem/tests.h>

namespace fieldTests {
	void setUpRadialScalarField(
		Field<double, Cell<MeshDim::D3>>& field,
		const Mesh<MeshDim::D3>& mesh,
		Point<GeometryDim::D3> center)
	{
		for (int i = 0; i < field.getElements()->values.length; i++) {
			double r = Vector<GeometryDim::D3>(center, mesh.getElements()->cells[i].centroid).getMagnitude();
			field.getElements()->values[i] = 1/r + 293;
		}
	}
}