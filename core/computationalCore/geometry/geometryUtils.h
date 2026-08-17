#pragma once

#include "vector.h"
#include <iostream>

namespace geometryOperations {

	// -------------------- Dot product ----------------------

	template<GeometryDim dim>
	double vecDotProduct(const Vector<dim>& vec1, const Vector<dim>& vec2) {
		double product = 0;
		for (int i = 0; i < geometryDimSize(dim); i++) {
			product = product + (vec1.comp[i] * vec2.comp[i]);
		}
		return product;
	};

	// -------------------- Cross product ----------------------

	template<GeometryDim dim>
	Vector<GeometryDim::D3> vecCrossProduct(const Vector<dim>& vec1, const Vector<dim>& vec2) {
		if (dim == GeometryDim::D2) {
			return Vector<GeometryDim::D3>({
				0,
				0,
				(vec1.comp[0] * vec2.comp[1]) - (vec1.comp[1] * vec2.comp[0])
				});
		}
		else if (dim == GeometryDim::D3) {
			return Vector<GeometryDim::D3>({
				(vec1.comp[1] * vec2.comp[2]) - (vec1.comp[2] * vec2.comp[1]),
				(vec1.comp[2] * vec2.comp[0]) - (vec1.comp[0] * vec2.comp[2]),
				(vec1.comp[0] * vec2.comp[1]) - (vec1.comp[1] * vec2.comp[0])
				});
		}
		else {
			return Vector<GeometryDim::D3>();
		}
	};

	// -------------------- Triangle area ----------------------

	template<GeometryDim dim>
	Vector<dim> calculateTriangleArea(const Point<dim>& p1, const Point<dim>& p2, const Point<dim>& p3) {
		Vector<dim> v1(p1), v2(p2), v3(p3);

		Vector<dim> area = vecCrossProduct((v2 - v1), (v3 - v1)) * 0.5;
		return area;
	};

	// -------------------- Move point with vector ----------------------

	template<GeometryDim dim>
	Point<dim> movePointWithVector(const Point<dim>& point, const Vector<dim>& vector) {

		Point<dim> resultPoint = Point<dim>();

		for (int i = 0; i < geometryDimSize(dim); i++) {
			resultPoint.pos[i] = point.pos[i] + vector.comp[i];
		};

		return resultPoint;
	};

	// -------------------- Calculate geocenter ----------------------

	template<GeometryDim dim>
	Point<dim> calculateGeoCenter(const std::vector<Point<dim>*>& points) {

		int numOfPoints = points.size();
		Point<dim> _geoCenter = Point<dim>();
		for (int i = 0; i < numOfPoints; i++) {
			_geoCenter = _geoCenter + *points[i];
		};
		return _geoCenter / (double)numOfPoints;
	};

	// -------------------- Linearity check ----------------------

	template<GeometryDim dim>
	bool arePointsColinear(const std::vector<Point<dim>*>& points) {
		assert(points.size() >= 3);
		
		// TODO: change that and below
		constexpr double tolerance = 0.001;

		bool areColinear = true;
		for (int i = 0; i < points.size() - 2; i++) {
			Vector<dim> vecA(*points[i], *points[i + 1]);
			Vector<dim> vecB(*points[i + 1], *points[i + 2]);
			Vector<GeometryDim::D3> result = geometryOperations::vecCrossProduct(vecA, vecB);
			double mag = result.getMagnitude();
			areColinear = (abs(mag) < tolerance);
			if (!areColinear) { break; };
		};
		return areColinear;
	}

	bool arePointsCoplanar(const std::vector<Point<GeometryDim::D3>*>& points) {
		assert(points.size() >= 4);

		using V = Vector<GeometryDim::D3>;

		constexpr double tolerance = 0.00001;
		bool foundPlane = false;

		V baseNVector = V();
		int basePointIndex = 0;

		for (int i = 0; i < points.size() - 2; i++) {
			for (int j = i + 1; j < points.size() - 1; j++) {
				for (int k = j + 1; k < points.size(); k++) {
					bool areColinear = geometryOperations::arePointsColinear<GeometryDim::D3>({ points[i], points[j], points[k] });
					if (!areColinear) {
						foundPlane = true;
						baseNVector = geometryOperations::vecCrossProduct(V(*points[i], *points[j]), V(*points[i], *points[k])).getNormal();
						basePointIndex = i;
						break;
					}
				}
				if (foundPlane) { break; }
			};
			if (foundPlane) { break; }
		};

		if (!foundPlane) { return true; }

		for (int i = 1; i < points.size(); i++) {
			if (i != basePointIndex) {
				V v(*points[basePointIndex], *points[i]);
				double dot = geometryOperations::vecDotProduct(baseNVector, v);
				if (abs(dot) > tolerance) {
					return false;
				}
			}

		}
		return true;
	}

}

// ------------------- Print geometry -----------------------

namespace geometryPrint {

	template<GeometryDim dim>
	void printP(const Point<dim>& point) {
		if (geometryDimSize(dim) == 2) {
			std::cout << point.pos[0] << ", " << point.pos[1] << std::endl;
		}
		else {
			std::cout << point.pos[0] << ", " << point.pos[1] << ", " << point.pos[2] << std::endl;
		}
	}

	template<GeometryDim dim>
	void printV(const Vector<dim>& vector) {
		if (geometryDimSize(dim) == 2) {
			std::cout << vector.comp[0] << ", " << vector.comp[1] << std::endl;
		}
		else {
			std::cout << vector.comp[0] << ", " << vector.comp[1] << ", " << vector.comp[2] << std::endl;
		}
	}

}