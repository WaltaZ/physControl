#pragma once

#include <problem/field.h>
#include <mesh/meshElements/meshElements.h>

namespace fieldTests {
	void setUpRadialScalarField(
		Field<double, Cell<MeshDim::D3>>& field, 
		const Mesh<MeshDim::D3>& mesh,
		Point<GeometryDim::D3> center);
}