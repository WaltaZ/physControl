#include "../../include/geometry/shapes/basic/volume.h"

Volume::Volume() {}

Volume::Volume(
		const std::vector<P*> points, 
		const std::vector<S*> surfaces, 
		const std::optional<int>& id) 
	: 
		surfaces(surfaces), 
		points(points), 
		id(id) {}

double Volume::getVolume() const
{

	P geoCenter = geometryOperations::calculateGeoCenter(points);
	double volume = 0;

	for (S* surface : surfaces) {
		const V& surfaceAreaVector = surface->getAreaVector();
		const P& surfaceCentroid = surface->getCentroid();

		V surfCentrToGeoCenter(surfaceCentroid, geoCenter);

		double pyramidArea = abs(geometryOperations::vecDotProduct(surfCentrToGeoCenter, surfaceAreaVector));

		volume = volume + pyramidArea;

	}
	return volume / 3.0;
}

Volume::P Volume::getCentroid() const
{
	P geoCenter = geometryOperations::calculateGeoCenter(points);
	P centroid = P();

	double volume = 0;

	for (S* surface : surfaces) {
		const V& surfaceAreaVector = surface->getAreaVector();
		const P& surfaceCentroid = surface->getCentroid();

		V surfCentrToGeoCenter(surfaceCentroid, geoCenter);

		P pyramidCentroid = geometryOperations::movePointWithVector(surfaceCentroid, surfCentrToGeoCenter * 0.25);
		double pyramidVolume = abs(geometryOperations::vecDotProduct(surfCentrToGeoCenter, surfaceAreaVector)) / 3.0;
		volume = volume + pyramidVolume;

		centroid = centroid + (pyramidCentroid * pyramidVolume);
	};

	return centroid / volume;
};