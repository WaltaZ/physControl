#pragma once

#include "../../geometryEnums.h"
#include "point.h"
#include "../../vector.h"
#include "../../geometryUtils.h"

#include <optional>

template<GeometryDim dim>
class Surface {
private:
	using V = Vector<GeometryDim::D3>;
	using P = Point<dim>;
public:

	std::vector<P*> vertices;
	std::optional<int> id;

	Surface(const std::vector<P*> vertices, const std::optional<int>& id = std::nullopt);

	P getCentroid() const;

	V getAreaVector() const;

	bool operator==(const Surface<dim>& surface) const;
};