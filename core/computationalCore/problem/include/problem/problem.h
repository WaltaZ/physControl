#pragma once

#include "problemGeometry.h"

#include <geometry/geometry.h>

namespace problem {

	class Problem {
	public:
		const GeometryDim problemDimensions;
	};

	class RoomHeatTransfer : public Problem {
	public:
		RoomHeatTransfer(

		);
	};
}