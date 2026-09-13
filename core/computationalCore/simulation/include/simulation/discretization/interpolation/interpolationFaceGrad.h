#pragma once

#include <geometry/vector.h>
#include <geometry/geometryUtils.h>

#include <mesh/meshElements/meshElements.h>
#include <problem/field/field.h>

namespace interpolation {

	__device__
		Vector<GeometryDim::D3> faceGradient(
			uint32_t f_id,
			Mesh<MeshDim::D3>* mesh,
			Field<double, Cell<MeshDim::D3>> phi,
			Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>> gradPhi
		);
}