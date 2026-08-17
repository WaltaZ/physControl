class Volume : Geometry {
protected:
	using P = Point<GeometryDim::D3>;
	using V = Vector<GeometryDim::D3>;
	using S = Surface<GeometryDim::D3>;
public:
	std::vector<P*> points;
	std::vector<S*> surfaces;
	std::optional<int> id;

	Volume(){};

	Volume( const std::vector<P*> points, 
			const std::vector<S*> surfaces,
			const std::optional<int>& id = std::nullopt
		) : surfaces(surfaces), points(points), id(id) {};

	double getVolume() {

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
	};

	P getCentroid() {
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

		return centroid/volume;
	};

};