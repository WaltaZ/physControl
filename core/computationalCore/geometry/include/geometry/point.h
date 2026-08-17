#pragma once

#include <array>

template <GeometryDim dim>
class Point : Geometry {
public:
	std::array<double, geometryDimSize(dim)> pos;

	Point() 
	{
		pos = std::array<double, geometryDimSize(dim)>{};
		std::fill(std::begin(pos), std::end(pos), 0);
	};

	Point(std::array<double, geometryDimSize(dim)> pos) : pos(pos) {};

	Point<dim> operator+(const Point<dim>& point) const
	{
		std::array<double, geometryDimSize(dim)> finalPos{};
		for (int i = 0; i < geometryDimSize(dim); i++) {
			finalPos[i] = pos[i] + point.pos[i];
		}
		return Point<dim>(finalPos);
	};

	Point<dim> operator-(const Point<dim>& point) const
	{
		std::array<double, geometryDimSize(dim)> finalPos{};
		for (int i = 0; i < geometryDimSize(dim); i++) {
			finalPos[i] = pos[i] - point.pos[i];
		}
		return Point<dim>(finalPos);
	};

	Point<dim> operator*(const double& scalar) const
	{
		std::array<double, geometryDimSize(dim)> finalComp{};
		for (int i = 0; i < geometryDimSize(dim); i++) {
			finalComp[i] = pos[i] * scalar;
		}
		return Point<dim>(finalComp);
	}

	Point<dim> operator/(const double& scalar) const
	{
		std::array<double, geometryDimSize(dim)> finalComp{};
		for (int i = 0; i < geometryDimSize(dim); i++) {
			finalComp[i] = pos[i] / scalar;
		}
		return Point<dim>(finalComp);
	}

	bool operator==(const Point<dim>& point) const
	{
		bool isTheSame = true;
		for (int i = 0; i < geometryDimSize(dim); i++) {
			isTheSame = (pos[i] == point.pos[i]);
			if (!isTheSame) { break; }
		}
		return isTheSame;
	}
};