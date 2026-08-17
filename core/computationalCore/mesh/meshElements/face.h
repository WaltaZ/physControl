template<MeshDim dim>
class Face : public MeshElement {
protected:
	using V = Vector<meshDimToGeometryDim(dim)>;
	using P = Point<meshDimToGeometryDim(dim)>;
public:
	std::vector<int> nodeIDs;
	int ownerCellID;
	std::optional<int> neighbourCellID;

	double area;
	V normalVector;
	P centroid;

	Face(
		double area,
		const V& normalVector,
		const P& centroid,
		const std::vector<int>& nodeIDs,
		int ownerCellID,
		std::optional<int> neighbourCellID = std::nullopt
	)
		: area(area),
		normalVector(normalVector),
		centroid(centroid),
		nodeIDs(nodeIDs),
		ownerCellID(ownerCellID),
		neighbourCellID(neighbourCellID) {};

	bool isBoundary() const {
		return !neighbourCellID.has_value();
	}
};

// TODO: Implement Boundary Face recognision.