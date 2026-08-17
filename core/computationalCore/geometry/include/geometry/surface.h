template<GeometryDim dim>
class Surface : Geometry {
private:
	using V = Vector<dim>;
	using P = Point<dim>;
public:

	std::vector<P*> vertices;

	Surface(const std::vector<P*> vertices) : vertices(vertices) {};

	P getCentroid() const {

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
		return centroid/area;
	}

	V getAreaVector() const {
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

	bool operator==(const Surface<dim>& surface) const 
	{
		bool isTheSame = true;
		for ( int i = 0; i < vertices.size(); i++) {
			isTheSame = (*vertices[i] == *surface.vertices[i]);
			if (!isTheSame) {
				break;
			}
		}
		return isTheSame;
	}
};

template<GeometryDim dim>
class SurfaceWithID : public Surface<dim> {
public:
	const int id;
	SurfaceWithID(const std::vector<Point<dim>*> vertices, const int id) : Surface<dim>(vertices), id(id) {};
};