#include "../../include/geometry/shapes/basic/surface.h"

template<GeometryDim dim>
Surface<dim>::Surface(const std::vector<P*> vertices, const std::optional<int>& id) : vertices(vertices), id(id) {}

template<GeometryDim dim>
Surface<dim>::P Surface<dim>::getCentroid() const
{
	// Calculating geocenter
	int numOfPoints = vertices.size();
	P geoCenter = geometryOperations::calculateGeoCenter(vertices);

	// Centroid
	P centroid = P();
	std::vector<double> triangleAreas{};

	for (int i = 0; i < numOfPoints; i++) {
		int index1 = i;
		int index2 = i + 1;
		if (i == numOfPoints - 1) {
			index2 = 0;
		}

		V triangleArea = geometryOperations::calculateTriangleArea(
			*vertices[index1],
			geoCenter,
			*vertices[index2]
		);

		triangleAreas.push_back(
			triangleArea.getMagnitude()
		);

		P triangleGeoCenter =
			geometryOperations::calculateGeoCenter(std::vector<P*>({
				vertices[index1],
				&geoCenter,
				vertices[index2]
				})
			);

		centroid = centroid + (triangleGeoCenter * triangleAreas[i]);
	}

	double area = std::accumulate(triangleAreas.begin(), triangleAreas.end(), 0.0);
	return centroid / area;
}

template<GeometryDim dim>
Surface<dim>::V Surface<dim>::getAreaVector() const
{
	int numOfPoints = vertices.size();
	V areaVector = V();
	P geoCenter = geometryOperations::calculateGeoCenter(vertices);
	for (int i = 0; i < numOfPoints; i++) {
		int index1 = i;
		int index2 = i + 1;
		if (i == numOfPoints - 1) {
			index2 = 0;
		}
		V triangleArea = geometryOperations::calculateTriangleArea(
			*vertices[index1],
			geoCenter,
			*vertices[index2]
		);
		areaVector = areaVector + triangleArea;
	}
	return areaVector;
}

template<GeometryDim dim>
bool Surface<dim>::operator==(const Surface<dim>& surface) const
{
	bool isTheSame = true;
	for (int i = 0; i < vertices.size(); i++) {
		isTheSame = (*vertices[i] == *surface.vertices[i]);
		if (!isTheSame) {
			break;
		}
	}
	return isTheSame;
}

template class Surface<GeometryDim::D2>;
template class Surface<GeometryDim::D3>;