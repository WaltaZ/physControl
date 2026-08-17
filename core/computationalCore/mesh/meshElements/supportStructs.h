template<GeometryDim dim>
struct NeighbourVectors {

	using V = Vector<dim>;

	V centroidToFace;
	V centroidToNeighbour;

	V centroidToFaceNormal;
	V centroidToNeighbourNormal;

	V Ef;
	V Tf;

};

struct NeighbourVectorsMagnitudes {

	double centroidToFace;
	double centroidToNeighbour;

	double centroidToFaceNormal;
	double centroidToNeighbourNormal;

	double Ef;
	double Tf;

};