template<GeometryDim dim>
class _Line : public Geometry {
protected:
	using P = Point<dim>;
	using V = Vector<dim>;
public:
	P* start;
	P* end;

	_Line(P* start, P* end) : start(start), end(end) {};
	virtual ~_Line() = default;

	P getCentroid() {
		return geometryOperations::calculateGeoCenter(std::vector<P*>({start, end}));
	}

	double getLength() {
		return V(*start, *end).getMagnitude();
	}
};

template <GeometryDim dim>
class Line : public _Line<dim> {};

template<>
class Line<GeometryDim::D2> : public _Line<GeometryDim::D2> {
public:
	Line(P* start, P* end) : _Line(start, end) {};

	V getAreaVector() {
		V lengthVector = V(*start, *end);
		return V({ -lengthVector.comp[1], lengthVector.comp[0] });
	}
};

template<>
class Line<GeometryDim::D3> : public _Line<GeometryDim::D3> {
public:
	Line(P* start, P* end) : _Line(start, end) {};
};

template<GeometryDim dim>
class LineWithID : public Line<dim> {
public:
	int id;
	LineWithID(int id, Point<dim>* start, Point<dim>* end) : Line<dim>(start, end), id(id) {};
};