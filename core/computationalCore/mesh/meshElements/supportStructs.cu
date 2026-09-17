#include "../include/mesh/meshElements/supportStructs.h"

// --------------------------------- VECTOR DATA ---------------------------------

template<GeometryDim dim>
__host__ __device__
VectorData<dim>::VectorData(){};

template<GeometryDim dim>
__host__ __device__
VectorData<dim>::VectorData(
	const V& vector,
	const V& normal,
	const double& magnitude) :
	vector(vector), normal(normal), magnitude(magnitude)
{};

template<GeometryDim dim>
__host__ __device__
VectorData<dim>::VectorData(Vector<dim> vector) :
	vector(vector), normal(vector.getNormal()), magnitude(vector.getMagnitude())
{};

template class VectorData<GeometryDim::D2>;
template class VectorData<GeometryDim::D3>;

template<GeometryDim dim>
void to_json(nlohmann::json& j, const VectorData<dim>& vectorData)
{
	j =
	{
		{"vector",		vectorData.vector},
		{"normal",		vectorData.normal},
		{"magnitude",	vectorData.magnitude}
	};
};

template void to_json(nlohmann::json& j, const VectorData<GeometryDim::D2>& vectorData);
template void to_json(nlohmann::json& j, const VectorData<GeometryDim::D3>& vectorData);

template<GeometryDim dim>
void from_json(const nlohmann::json& j, VectorData<dim>& vectorData)
{
	j.at("vector").get_to(vectorData.vector);
	j.at("normal").get_to(vectorData.normal);
	j.at("magnitude").get_to(vectorData.magnitude);
};

template void from_json(const nlohmann::json& j, VectorData<GeometryDim::D2>& vectorData);
template void from_json(const nlohmann::json& j, VectorData<GeometryDim::D3>& vectorData);

// ---------------------------------- CELL DATA ----------------------------------

template<GeometryDim dim>
void to_json(nlohmann::json& j, const CellData<dim>& cellData)
{
	j =
	{
		{"centroidToFace",		cellData.centroidToFace},
		{"Ef",					cellData.Ef},
		{"Tf",					cellData.Tf}	
	};
};

template void to_json(nlohmann::json& j, const CellData<GeometryDim::D2>& cellData);
template void to_json(nlohmann::json& j, const CellData<GeometryDim::D3>& cellData);

template<GeometryDim dim>
void from_json(const nlohmann::json& j, CellData<dim>& cellData)
{
	j.at("centroidToFace").get_to(cellData.centroidToFace);
	j.at("Ef").get_to(cellData.Ef);
	j.at("Tf").get_to(cellData.Tf);
};

template void from_json(const nlohmann::json& j, CellData<GeometryDim::D2>& cellData);
template void from_json(const nlohmann::json& j, CellData<GeometryDim::D3>& cellData);