#include <problem/tests.h>

#include <cmath>

namespace fieldTests {

	void setUpRadialField(
		Field<double, Cell<MeshDim::D3>>* field,
		const Mesh<MeshDim::D3>* mesh,
		Point<GeometryDim::D3> center,
		double initValue)
	{
		for (int i = 0; i < field->values.length; i++) {
			double r = V(center, mesh->cells[i].centroid).getMagnitude();
			field->values[i] = 1/r + initValue;
		}
	}

	void setUpRadialField(
		Field<V, Cell<MeshDim::D3>>* field,
		const Mesh<MeshDim::D3>* mesh,
		Point<GeometryDim::D3> center,
		V initVec)
	{
		for (int i = 0; i < field->values.length; i++) {
			V r = V(center, mesh->cells[i].centroid);
			field->values[i] = 1/(r.getMagnitude()) * r.getNormal() + initVec;
		}
	}

	void fieldTests::setUpCurlyField(
		Field<V, Cell<MeshDim::D3>>* field, 
		const Mesh<MeshDim::D3>* mesh, 
		Point<GeometryDim::D3> center,
		double a,
		double b,
		V initValue)
	{

		constexpr double PI = 3.14159265358979323846;

		for (size_t i = 0; i < field->values.length; i++)
		{
			const auto& c = mesh->cells[i].centroid;

			const double kx = PI / a;
			const double ky = PI / b;

			const double sx = sin(kx * c.pos[0]);
			const double sy = sin(ky * c.pos[1]);

			const double u = ky * sx * sx * sin(2.0 * ky * c.pos[1]);
			const double v = -kx * sin(2.0 * kx * c.pos[0]) * sy * sy;

			field->values[i] = V({ u, v, 0 }) * 2;
		}
	}
}