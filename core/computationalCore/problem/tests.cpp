#include <problem/tests.h>

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
		V initValue)
	{
		for (size_t i = 0; i < field->values.length; i++)
		{
			const auto& C_centroid = mesh->cells[i].centroid;

			V curl = V({ C_centroid.pos[1] - center.pos[1], center.pos[0] - C_centroid.pos[0], 0 });
			field->values[i] = curl * 0.25;
		}
	}
}