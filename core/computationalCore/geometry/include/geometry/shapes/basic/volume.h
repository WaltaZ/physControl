#pragma once

#include "../../geometryEnums.h"
#include "../../point.h"
#include "../../vector.h"
#include "../../geometryUtils.h"
#include "surface.h"

#include <optional>

class Volume {
protected:
	using P = Point<GeometryDim::D3>;
	using V = Vector<GeometryDim::D3>;
	using S = Surface<GeometryDim::D3>;
public:
	std::vector<P*> points;
	std::vector<S*> surfaces;
	std::optional<int> id;

	Volume();

	Volume(
		const std::vector<P*> points,
		const std::vector<S*> surfaces,
		const std::optional<int>& id = std::nullopt);

	double getVolume() const;

	P getCentroid() const;
};