#include "../../include/geometry/point.h"

template<GeometryDim dim>
Point<dim>::Point() {
	pos = std::array<double, geometryDimSize(dim)>{};
	std::fill(std::begin(pos), std::end(pos), 0);
}

template<GeometryDim dim>
Point<dim> Point<dim>::operator+(const Point<dim>& point) const
{
	std::array<double, geometryDimSize(dim)> finalPos{};
	for (int i = 0; i < geometryDimSize(dim); i++) {
		finalPos[i] = pos[i] + point.pos[i];
	}
	return Point<dim>(finalPos);
}

template<GeometryDim dim>
Point<dim> Point<dim>::operator-(const Point<dim>& point) const
{
	std::array<double, geometryDimSize(dim)> finalPos{};
	for (int i = 0; i < geometryDimSize(dim); i++) {
		finalPos[i] = pos[i] - point.pos[i];
	}
	return Point<dim>(finalPos);
}
template<GeometryDim dim>
Point<dim> Point<dim>::operator*(const double& scalar) const
{
	std::array<double, geometryDimSize(dim)> finalComp{};
	for (int i = 0; i < geometryDimSize(dim); i++) {
		finalComp[i] = pos[i] * scalar;
	}
	return Point<dim>(finalComp);
}

template<GeometryDim dim>
Point<dim> Point<dim>::operator/(const double& scalar) const
{
	std::array<double, geometryDimSize(dim)> finalComp{};
	for (int i = 0; i < geometryDimSize(dim); i++) {
		finalComp[i] = pos[i] / scalar;
	}
	return Point<dim>(finalComp);
}

template<GeometryDim dim>
bool Point<dim>::operator==(const Point<dim>& point) const
{
	bool isTheSame = true;
	for (int i = 0; i < geometryDimSize(dim); i++) {
		isTheSame = (pos[i] == point.pos[i]);
		if (!isTheSame) { break; }
	}
	return isTheSame;
}

template class Point<GeometryDim::D2>;
template class Point<GeometryDim::D3>;