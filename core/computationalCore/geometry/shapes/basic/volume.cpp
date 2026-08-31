#include "../../include/geometry/shapes/basic/volume.h"
#include <unordered_map>

Volume::Volume() {}

Volume::Volume(
		const std::vector<P*> points, 
		const std::vector<S*> surfaces, 
		const std::optional<int>& id) 
	: 
		surfaces(surfaces), 
		points(points), 
		id(id) {}

Volume::~Volume()
{
	for (S* surface : surfaces) {
		delete surface;
	}
	for (P* point : points) {
		delete point;
	}
}

Volume::Volume(const Volume& other)
	: id(other.id)
{
	std::unordered_map<P*, P*> otherToThisPoints;
	points.reserve(other.points.size());

	for (P* point : other.points) {
		P* newPoint = new P(*point);
		points.push_back(newPoint);
		otherToThisPoints.emplace(point, newPoint);
	};

	surfaces.reserve(other.surfaces.size());

	for (S* surface : other.surfaces) {
		std::vector<P*> newVertices;
		newVertices.reserve(surface->vertices.size());

		for (P* vertex : surface->vertices) {
			newVertices.push_back(otherToThisPoints.at(vertex));
		}

		surfaces.push_back(new S(newVertices, surface->id));
	}

}

Volume::Volume(Volume&& other) noexcept
	: points(std::move(other.points)), surfaces(std::move(other.surfaces)),
	id(std::move(other.id))
{
	other.points.clear();
	other.surfaces.clear();
	other.id.reset();
}

Volume& Volume::operator=(const Volume& other)
{
	if (this != &other) {
		Volume temp(other); 

		std::swap(points, temp.points); 
		std::swap(surfaces, temp.surfaces); 
		std::swap(id, temp.id);
	}

	return *this;
}

Volume& Volume::operator=(Volume&& other) noexcept
{
	if (this != &other) {
		for (P* point : points) {
			delete point;
		}
		for (S* surface : surfaces) {
			delete surface;
		}

		points = std::move(other.points);
		surfaces = std::move(other.surfaces);
		id = std::move(other.id);

		other.points.clear();
		other.surfaces.clear();
		other.id.reset();
	}

	return *this;
}

double Volume::getVolume() const
{

	P geoCenter = geomOp::calculateGeoCenter(points);
	double volume = 0;

	for (S* surface : surfaces) {
		const V& surfaceAreaVector = surface->getAreaVector();
		const P& surfaceCentroid = surface->getCentroid();

		V surfCentrToGeoCenter(surfaceCentroid, geoCenter);

		double pyramidArea = abs(geomOp::vecDotProduct(surfCentrToGeoCenter, surfaceAreaVector));

		volume = volume + pyramidArea;

	}
	return volume / 3.0;
}

Volume::P Volume::getCentroid() const
{
	P geoCenter = geomOp::calculateGeoCenter(points);
	P centroid = P();

	double volume = 0;

	for (S* surface : surfaces) {
		const V& surfaceAreaVector = surface->getAreaVector();
		const P& surfaceCentroid = surface->getCentroid();

		V surfCentrToGeoCenter(surfaceCentroid, geoCenter);

		P pyramidCentroid = geomOp::movePointWithVector(surfaceCentroid, surfCentrToGeoCenter * 0.25);
		double pyramidVolume = abs(geomOp::vecDotProduct(surfCentrToGeoCenter, surfaceAreaVector)) / 3.0;
		volume = volume + pyramidVolume;

		centroid = centroid + (pyramidCentroid * pyramidVolume);
	};

	return centroid / volume;
};