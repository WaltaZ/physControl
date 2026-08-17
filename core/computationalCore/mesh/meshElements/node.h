template <MeshDim dim>
struct Node : public MeshElement{
	std::array<double, meshDimSize(dim)> pos;

	Node(const std::array<double, meshDimSize(dim)>& pos) : pos(pos) {};
	Node(const Point<meshDimToGeometryDim(dim)>& point) : pos(point.pos) {};
};