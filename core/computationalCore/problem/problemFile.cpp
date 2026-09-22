#include <problem/problem.h>

#include <fileStream/binaryFiles/binStream.h>

template<class A, class B>
void _saveBoundaryGeometry(
	const std::string& path,
	std::vector<BoundaryConditionD3>& BCs,
	const Field<A, B>* field
) 
{
	if (BCs.size() == 0) { return; }
	
	const std::string fileName = field->name + "_BGeom.bin";

	BinWriter bGeomFile{ path, fileName };

	bGeomFile.writeObj(static_cast<uint32_t>(BCs.size()));

	for (auto& bc : BCs) 
	{
		bc.geometry.save(bGeomFile);
	}
};

template<class A, class B>
void _loadBoundaryGeometry(
	const std::string& path,
	std::vector<BoundaryConditionD3>& BCs,
	const Field<A, B>* field
)
{
	const std::string fileName = field->name + "_BGeom.bin";

	BinReader bGeomFile{ path, fileName, false };

	try {
		bGeomFile.open();
	}
	catch (...) { return; }

	uint32_t BCsLength;
	bGeomFile.readObj(BCsLength);

	BCs.reserve(BCsLength);

	for (size_t i = 0; i < BCsLength; i++)
	{
		const auto& bp = field->boundaryPatches[i];

		BCs.emplace_back(
			bp.type,
			std::vector<double>(
				bp.values.getData(),
				bp.values.getData() + bp.values.length
			),
			Surface<GeometryDim::D3>::load(bGeomFile)
		);
	}
}

void HeatTransferProblemD3::save(const std::string& path)
{
	fields->velocity->saveBoundaryPatches(path);
	_saveBoundaryGeometry(path, boundaryConditions[0], fields->velocity);

	fields->temperature->saveBoundaryPatches(path);
	_saveBoundaryGeometry(path, boundaryConditions[1], fields->temperature);

	BinWriter f_problem{ path, "problem.bin" };

	geometry.save(f_problem);

}

void HeatTransferProblemD3::load(const std::string& path)
{
	fields->velocity->loadBoundaryPatches(path);
	_loadBoundaryGeometry(path, boundaryConditions[0], fields->velocity);

	fields->temperature->loadBoundaryPatches(path);
	_loadBoundaryGeometry(path, boundaryConditions[1], fields->temperature);

	BinReader f_problem{ path, "problem.bin" };

	geometry = Volume::load(f_problem);
}