#pragma once

#include "vector.h"
#include "point.h"
#include "matrixTensor.h"

#include <iostream>
#include <vector>
#include <assert.h>
#include <utility/cudaUtils.h>

namespace geomOp {

	using V2 = Vector<GeometryDim::D2>;
	using V3 = Vector<GeometryDim::D3>;

	using P2 = Point<GeometryDim::D2>;
	using P3 = Point<GeometryDim::D3>;

	using T2 = MatrixTensor<GeometryDim::D2>;
	using T3 = MatrixTensor<GeometryDim::D3>;

	template<GeometryDim dim>
	__host__ __device__
	double dotProduct(const Vector<dim>& vec1, const Vector<dim>& vec2);

	template<GeometryDim dim>
	__host__ __device__
	Vector<dim> dotProduct(const MatrixTensor<dim>& tensor, const Vector<dim>& vec);

	Vector<GeometryDim::D3> crossProduct(const V2& vec1, const V2& vec2);
	Vector<GeometryDim::D3> crossProduct(const V3& vec1, const V3& vec2);

	template<GeometryDim dim>
	V3 calculateTriangleArea(const Point<dim>& p1, const Point<dim>& p2, const Point<dim>& p3);

	template<GeometryDim dim>
	Point<dim> movePointWithVector(const Point<dim>& point, const Vector<dim>& vector);

	template<GeometryDim dim>
	Point<dim> calculateGeoCenter(const std::vector<Point<dim>*>& points);
	
	template<GeometryDim dim>
	bool arePointsColinear(const std::vector<Point<dim>*>& points);

	bool arePointsCoplanar(const std::vector<Point<GeometryDim::D3>*>& points);

}

namespace geomUtils {
	template<typename T>
	__host__ __device__
	CudaArray<double> getComponents(T* obj);
}

// ------------------- Print geometry -----------------------

namespace geomPrint {

	void printP(const Point<GeometryDim::D2>& point);

	void printV(const Vector<GeometryDim::D2>& vector);

	void printP(const Point<GeometryDim::D3>& point);

	void printV(const Vector<GeometryDim::D3>& vector);

}