#pragma once

#include <array>
#include "../../geometryEnums.h"

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
};