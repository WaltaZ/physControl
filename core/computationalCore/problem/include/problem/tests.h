#pragma once

#include <problem/field/field.h>
#include <mesh/meshElements/meshElements.h>

namespace fieldTests {

	using V = Vector<GeometryDim::D3>;

	void setUpRadialField(
		Field<double, Cell<MeshDim::D3>>* field, 
		const Mesh<MeshDim::D3>* mesh,
		Point<GeometryDim::D3> center,
		double initValue = 0
	);

	void setUpRadialField(
		Field<V, Cell<MeshDim::D3>>* field, 
		const Mesh<MeshDim::D3>* mesh,
		Point<GeometryDim::D3> center,
		V initValue = V()
	);

	void setUpCurlyField(
		Field<V, Cell<MeshDim::D3>>* field, 
		const Mesh<MeshDim::D3>* mesh,
		Point<GeometryDim::D3> center,
		V initValue = V()
	);
}