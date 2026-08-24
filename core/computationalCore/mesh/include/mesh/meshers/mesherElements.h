#pragma once

#include "../meshEnums.h"
#include "../meshElements/supportStructs.h"

#include <vector>
#include <geometry/geometry.h>

	template <MeshDim dim>
	struct MesherNode {
		std::array<double, meshDimSize(dim)> pos;

		MesherNode(const std::array<double, meshDimSize(dim)>& pos);
		MesherNode(const Point<meshDimToGeometryDim(dim)>& point);
	};

	template<MeshDim dim>
	class MesherFace {
	protected:
		using V = VectorData<meshDimToGeometryDim(dim)>;
		using P = Point<meshDimToGeometryDim(dim)>;
	public:
		V area;
		P centroid;

		std::vector<int> nodeIDs;

		int ownerCellID;
		CellData<meshDimToGeometryDim(dim)> ownerData;

		std::optional<int> neighbourCellID = std::nullopt;
		std::optional<CellData<meshDimToGeometryDim(dim)>> neighbourData = std::nullopt;

		std::optional<V> ownerToNeighbourCell = std::nullopt;
	};

	template<MeshDim dim>
	class MesherCell {
	private:
		using P = Point<meshDimToGeometryDim(dim)>;
	public:
		std::vector<int> nodeIDs;
		std::vector<int> faceIDs;
		std::vector<int> neighbourCellsIDs;

		P centroid;
		double volume;
	};

	template<MeshDim dim>
	struct MesherMesh {
		std::vector<MesherNode<dim>> nodes{};
		std::vector<MesherFace<dim>> faces{};
		std::vector<MesherCell<dim>> cells{};
	};