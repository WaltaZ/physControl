#pragma once

#include <geometry/vector.h>
#include <geometry/geometryUtils.h>

namespace interpolation {

	template<class Obj, class GradObj>
	__device__
		Obj taylor(
			const Obj& phi,
			const GradObj& gradPhi,
			const Vector<GeometryDim::D3>& direction
		);
}