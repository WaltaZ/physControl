#pragma once

#include "geometryEnums.h"

#include <array>
#include <nlohmann/json.hpp>

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

template<GeometryDim dim>
void to_json(nlohmann::json& json, const Point<dim>& point);

template<GeometryDim dim>
void from_json(const nlohmann::json& json, Point<dim>& point);