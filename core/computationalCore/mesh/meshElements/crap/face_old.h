#pragma once
#pragma message(">>> ENTERING face.h")

#include <vector>
#include <math.h>
#include <numeric>
#include <iostream>

#include <meshTypes.h>

#include "point.h"
#include "vector.h"
#include "geometryUtils.h"

// _Face abstract class --------------------------------------------------------
template <MeshDim dim>
class _Face {
private:
	using V = Vector<dim>;
	using P = Point<dim>;

protected:
	P _calculateGeoCenter(const std::vector<P>& points) {

		// Calculating geocenter
		int numOfPoints = points.size();
		P _geoCenter = P();
		for (int i = 0; i < numOfPoints; i++) {
			_geoCenter = _geoCenter + points[i];
		};
		return _geoCenter / (double)numOfPoints;
	}

public:
	bool isInitialized = false;
	double area;
	P centroid;
	P geoCenter;
	V normalVector;

	const std::vector<int> pointIDs;
	_Face(std::vector<int> pointIDs) : pointIDs(pointIDs) {};

	virtual void initParams(const std::vector<Point<dim>>& allPoints) = 0;
};

// Face class --------------------------------------------------------------
template<MeshDim dim>
class Face : public _Face<dim> {};

// 2D Face class -----------------------------------------------------------
template<>
class Face<MeshDim::D2> : public _Face<MeshDim::D2> {
private:
	using V = Vector<MeshDim::D2>;
	using P = Point<MeshDim::D2>;

public:
	Face(std::vector<int> pointIDs) : _Face(pointIDs) {};

	void initParams(const std::vector<P>& allPoints) override {
		// TODO: Check if it works

		std::vector<P> points{};
		for (int i = 0; i < 2; i++) {
			points.push_back(allPoints[pointIDs[i]]);
		}
		
		// Geometry Center
		geoCenter = _calculateGeoCenter(points);

		// Centroid
		centroid = geoCenter;

		// Area
		V vectorBetweenPoints(points.front(), points.back());
		area = vectorBetweenPoints.getMagnitude();

		// Normal
		V vectorBetweenPointsNormal = vectorBetweenPoints / area;
		normalVector = V({
			-vectorBetweenPointsNormal.comp[1],
			vectorBetweenPointsNormal.comp[0] 
		});

		isInitialized = true;
	}
};

// 3D Face class -----------------------------------------------------------
template<>
class Face<MeshDim::D3> : public _Face<MeshDim::D3> {
private:
	using V = Vector<MeshDim::D3>;
	using P = Point<MeshDim::D3>;

protected:
	V _calculateTriangleArea(const P& p1, const P& p2, const P& p3) {
		V v1(p1), v2(p2), v3(p3);

		V area = geometry::crossProduct((v2 - v1), (v3 - v1)) * 0.5;
		return area;
	}
public:
	Face(std::vector<int> pointIDs) : _Face(pointIDs) {};

	void initParams(const std::vector<P>& allPoints) override {

		int numOfPoints = pointIDs.size();
		std::vector<P> points{};
		for (int i = 0; i < numOfPoints; i++) {
			points.push_back(allPoints[pointIDs[i]]);
		}

		// Geometry Center
		geoCenter = _calculateGeoCenter(points);

		// Area and centroid
		std::vector<double> triangleAreas{};
		centroid = P();

		for (int i = 0; i < numOfPoints; i++) {
			int index1 = i;
			int index2 = i + 1;
			if (i == numOfPoints - 1) {
				index2 = 0;
			}

			V triangleArea = _calculateTriangleArea(
				points[index1],
				geoCenter,
				points[index2]
			);

			if (i == 0) {
				normalVector = triangleArea.getNormal();
			}

			triangleAreas.push_back(
				triangleArea.getMagnitude()
			);

			P triangleGeoCenter = 
				_calculateGeoCenter(std::vector<P>({
					points[index1],
					geoCenter,
					points[index2]
				})
			);

			centroid = centroid + (triangleGeoCenter * triangleAreas[i]);
		}

		area = std::accumulate(triangleAreas.begin(), triangleAreas.end(), 0.0);
		centroid = centroid / area;
		isInitialized = true;
	}
};

// TODO: Add something defining boundary conditions for faces.

template<MeshDim dim>
class BoundaryFace : public Face<dim> {};

template<>
class BoundaryFace<MeshDim::D2> : public Face<MeshDim::D2> {
public:
	BoundaryFace(std::vector<int> pointIDs) : Face(pointIDs) {};
};

template<>
class BoundaryFace<MeshDim::D3> : public Face<MeshDim::D3> {
public:
	BoundaryFace(std::vector<int> pointIDs) : Face(pointIDs) {};
};