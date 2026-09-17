#include "../include/mesh/meshers/mesherElements.h"

// --------------------------------------- NODE --------------------------------------------

template<MeshDim dim>
MesherNode<dim>::MesherNode(const std::array<double, meshDimSize(dim)>& pos) : pos(pos) {};

template<MeshDim dim>
MesherNode<dim>::MesherNode(const Point<mesh2geom(dim)>& point) : pos(point.pos) {};

template<MeshDim dim>
void to_json(nlohmann::json& j, const MesherNode<dim>& node) 
{
	j = 
	{
		{"pos", node.pos}
	};
};

template void to_json(nlohmann::json& j, const MesherNode<MeshDim::D2>& node);
template void to_json(nlohmann::json& j, const MesherNode<MeshDim::D3>& node);

template<MeshDim dim>
void from_json(const nlohmann::json& j, MesherNode<dim>& node) 
{
	j.at("pos").get_to(node.pos);
};

template void from_json(const nlohmann::json& j, MesherNode<MeshDim::D2>& node);
template void from_json(const nlohmann::json& j, MesherNode<MeshDim::D3>& node);

template class MesherNode<MeshDim::D2>;
template class MesherNode<MeshDim::D3>;

// --------------------------------------- FACE -------------------------------------------

template<MeshDim dim>
void to_json(nlohmann::json& j, const MesherFace<dim>& face)
{
	j =
	{
		{"area",					face.area},
		{"centroid",				face.centroid},
		{"nodeIDs",					face.nodeIDs},
		{"ownerCellID",				face.ownerCellID},
		{"ownerData",				face.ownerData},
		{"neighbourCellID",			face.neighbourCellID},
		{"neighbourData",			face.neighbourData},
		{"ownerToNeighbourCell",	face.ownerToNeighbourCell}

	};
};

template void to_json(nlohmann::json& j, const MesherFace<MeshDim::D2>& face);
template void to_json(nlohmann::json& j, const MesherFace<MeshDim::D3>& face);

template<MeshDim dim>
void from_json(const nlohmann::json& j, MesherFace<dim>& face)
{
	j.at("area").get_to(face.area);
	j.at("centroid").get_to(face.centroid);
	j.at("nodeIDs").get_to(face.nodeIDs);
	j.at("ownerCellID").get_to(face.ownerCellID);
	j.at("ownerData").get_to(face.ownerData);
	j.at("neighbourCellID").get_to(face.neighbourCellID);
	j.at("neighbourData").get_to(face.neighbourData);
	j.at("ownerToNeighbourCell").get_to(face.ownerToNeighbourCell);
};

template void from_json(const nlohmann::json& j, MesherFace<MeshDim::D2>& face);
template void from_json(const nlohmann::json& j, MesherFace<MeshDim::D3>& face);

// ------------------------------------ CELL ----------------------------------------

template<MeshDim dim>
void to_json(nlohmann::json& j, const MesherCell<dim>& cell)
{
	j =
	{
		{"nodeIDs",				cell.nodeIDs},
		{"faceIDs",				cell.faceIDs},
		{"neighbourCellsIDs",	cell.neighbourCellsIDs},
		{"centroid",			cell.centroid},
		{"volume",				cell.volume}
	};
};

template void to_json(nlohmann::json& j, const MesherCell<MeshDim::D2>& cell);
template void to_json(nlohmann::json& j, const MesherCell<MeshDim::D3>& cell);

template<MeshDim dim>
void from_json(const nlohmann::json& j, MesherCell<dim>& cell)
{
	j.at("nodeIDs").get_to(cell.nodeIDs);
	j.at("faceIDs").get_to(cell.faceIDs);
	j.at("neighbourCellsIDs").get_to(cell.neighbourCellsIDs);
	j.at("centroid").get_to(cell.centroid);
	j.at("volume").get_to(cell.volume);
};

template void from_json(const nlohmann::json& j, MesherCell<MeshDim::D2>& cell);
template void from_json(const nlohmann::json& j, MesherCell<MeshDim::D3>& cell);

// ------------------------------------ MESH ----------------------------------------

template<MeshDim dim>
void to_json(nlohmann::json& j, const MesherMesh<dim>& mesh)
{
	j =
	{
		{"nodes",						mesh.nodes},
		{"faces",						mesh.faces},
		{"cells",						mesh.cells},
		{"boundaryConditions",			mesh.boundaryConditions},
		{"boundaryConditionsDefault",	mesh.boundaryConditionsDefault}
	};
};

template void to_json(nlohmann::json& j, const MesherMesh<MeshDim::D2>& mesh);
template void to_json(nlohmann::json& j, const MesherMesh<MeshDim::D3>& mesh);

template<MeshDim dim>
void from_json(const nlohmann::json& j, MesherMesh<dim>& mesh)
{
	j.at("nodes").get_to(mesh.nodes);
	j.at("faces").get_to(mesh.faces);
	j.at("cells").get_to(mesh.cells);
	j.at("boundaryConditions").get_to(mesh.boundaryConditions);
	j.at("boundaryConditionsDefault").get_to(mesh.boundaryConditionsDefault);
};

template void from_json(const nlohmann::json& j, MesherMesh<MeshDim::D2>& mesh);
template void from_json(const nlohmann::json& j, MesherMesh<MeshDim::D3>& mesh);

// ------------------------------- Boundary Conditions ------------------------------

MesherBoundaryCondition::MesherBoundaryCondition(
	const Cuboid::FaceType face,
	const std::array<std::array<double, 2>, 2> range
):
	face(face),
	range(range)
{};

void to_json(nlohmann::json& j, const MesherBoundaryConditionRaw& bc) 
{
	j = {
		{"faceIDs", bc.faceIDs}
	};
};

void from_json(const nlohmann::json& j, MesherBoundaryConditionRaw& bc) 
{
	j.at("faceIDs").get_to(bc.faceIDs);
};

void to_json(nlohmann::json& j, const MesherBoundaryCondition& bc) 
{
	j = {
		{"faceIDs", bc.faceIDs}
	};
};

void from_json(const nlohmann::json& j, MesherBoundaryCondition& bc) 
{
	j.at("faceIDs").get_to(bc.faceIDs);
};