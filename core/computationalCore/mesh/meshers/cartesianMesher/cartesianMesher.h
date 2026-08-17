template<MeshDim>
class _CartesianMesher {};

template<MeshDim dim>
class CartesianMesher : _CartesianMesher<dim> {};

template<>
class CartesianMesher<MeshDim::D3> : _CartesianMesher<MeshDim::D3> {
private:
	const static GeometryDim Gdim = GeometryDim::D3;
	const ProblemGeometry<Gdim, Cuboid>& problemGeometry;
	std::array<int, geometryDimSize(Gdim)> refinments;
	std::vector<std::vector<std::vector<double>>> divisionPatterns{};

	std::vector<double> _linspace(const int& index) const {

		const int& refinment = refinments[index];

		double interval = 1.0 / (double)refinment;
		std::vector<double> result(refinment);
		for (int i = 0; i < refinment; i++) {
			result[i] = (i + 1) * interval;
		}
		return result;
	}

public:

	CartesianMesher(
			const ProblemGeometry<Gdim, Cuboid>& problemGeometry,
			const std::array<int, geometryDimSize(Gdim)>& refinments = { 10, 10, 10 })
		:	
			problemGeometry(problemGeometry), 
			refinments(refinments) 
	{
			// TODO: Fix that \/
			//constexpr bool isCuboid = std::is_same_v<decltype(problemGeometry.operationalSpace), Cuboid>;
			//assert(isCuboid);
		for (int i = 0; i < problemGeometry.operationalSpace.size(); i++) {
			std::vector<std::vector<double>> currentCuboidDivision;
			for (int j = 0; j < geometryDimSize(Gdim); j++) {
				currentCuboidDivision.push_back(_linspace(j));
			}
			divisionPatterns.push_back(currentCuboidDivision);
		}
	}

	void setDivisionPattern(const std::vector<std::vector<double>>& divisionPattern, int index) {
		for (int i = 0; i < divisionPattern.size(); i++) {
			divisionPatterns[index][i] = divisionPattern[i];
		}
	}

	Mesh<MeshDim::D3> createMesh() {

		const std::vector<std::vector<double>>& divisionPattern = divisionPatterns[0];
		const Cuboid& cuboid = problemGeometry.operationalSpace[0];

		Mesh<MeshDim::D3> mesh = Mesh<MeshDim::D3>();

		// Nodes
		for (int z = 0; z <= refinments[2]; z++) {
			for (int y = 0; y <= refinments[1]; y++) {
				for (int x = 0; x <= refinments[0]; x++) {
					Point<Gdim> point({
						cuboid.points[0]->pos[0] + (x * (cuboid.a / (double)refinments[0])),
						cuboid.points[0]->pos[1] + (y * (cuboid.b / (double)refinments[1])),
						cuboid.points[0]->pos[2] + (z * (cuboid.c / (double)refinments[2]))
					});
					mesh.nodes.push_back(Node<MeshDim::D3>(point));
				}
			}
		}



		return mesh;
	};
};