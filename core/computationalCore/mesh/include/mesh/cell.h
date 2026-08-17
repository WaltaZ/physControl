template<MeshDim dim>
class Cell : public MeshElement {
private:
	using P = Point<meshDimToGeometryDim(dim)>;
	using V = Vector<meshDimToGeometryDim(dim)>;
	//using F = Face<meshDimToGeometryDim(dim)>;
public:
	std::vector<int> pointIDs;
	std::vector<int> faceIDs;
	std::vector<int> neighbourCellsIDs;

	P centroid;
	double volume;

	std::vector<NeighbourVectors<meshDimToGeometryDim(dim)>> neighboursVectors;
	std::vector<NeighbourVectorsMagnitudes> neighboursVectorsMagnitudes;
	//std::vector<int> faceNormalsOrient;

	Cell(
		std::vector<int> pointIDs,
		std::vector<int> faceIDs,
		std::vector<int> neighbourCellsIDs,
		P centroid,
		double volume,
		std::vector<NeighbourVectors<meshDimToGeometryDim(dim)>> neighboursVectors,
		std::vector<NeighbourVectorsMagnitudes> neighboursVectorsMagnitudes,
		std::vector<int> faceNormalsOrient
	) :
		pointIDs(pointIDs),
		faceIDs(faceIDs),
		neighbourCellsIDs(neighbourCellsIDs),
		centroid(centroid),
		volume(volume),
		neighboursVectors(neighboursVectors),
		neighboursVectorsMagnitudes(neighboursVectorsMagnitudes)
	{};
};