#pragma once

#include "../../geometryEnums.h"
#include "../../geometryUtils.h"

#include<optional>

template<GeometryDim dim>
class _Line {
protected:
	using P = Point<dim>;
	using V = Vector<dim>;
public:
	P* start;
	P* end;
	std::optional<int> id;

	_Line(P* start, P* end, const std::optional<int>& id = std::nullopt);
	virtual ~_Line();

	P getCentroid();

	double getLength();
};

template <GeometryDim dim>
class Line : public _Line<dim> {};

template<>
class Line<GeometryDim::D2> : public _Line<GeometryDim::D2> {
public:

	Line(P* start, P* end, const std::optional<int>& id = std::nullopt);
	~Line() override;

	V getAreaVector();
};

template<>
class Line<GeometryDim::D3> : public _Line<GeometryDim::D3> {
public:
	Line(P* start, P* end, const std::optional<int>& id = std::nullopt);
};