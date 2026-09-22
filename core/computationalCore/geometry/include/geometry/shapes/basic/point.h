#pragma once

#include "../../geometryEnums.h"

#include <array>
#include <fileStream/binaryFiles/binStream.h>

template <GeometryDim dim>
class Point {
public:
	std::array<double, geometryDimSize(dim)> pos;

	Point();

	Point(std::array<double, geometryDimSize(dim)> pos) : pos(pos) {};

	Point<dim> operator+(const Point<dim>& point) const;

	Point<dim> operator-(const Point<dim>& point) const;

	Point<dim> operator*(const double& scalar) const;

	Point<dim> operator/(const double& scalar) const;

	bool operator==(const Point<dim>& point) const;

	//Point(Point<dim>&& other);

	void save(BinWriter& writer);
	static Point<dim> load(BinReader& reader);
};

template class Point<GeometryDim::D2>;
template class Point<GeometryDim::D3>;