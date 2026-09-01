#include "include/problem/problem.h"

ProblemD3::ProblemD3(
	const ProblemGeometryD3& geometry) : geometry(geometry) {
}

HeatTransferProblemD3::HeatTransferProblemD3(
	const ProblemGeometryD3& geometry) : ProblemD3(geometry)
{
	boundaryConditions = std::vector<std::vector<BoundaryConditionD3>>{ 2 };
	defaultBoundaryConditions = std::vector<BoundaryConditionD3Raw>{
		BoundaryConditionD3Raw(BoundaryConditionType::NoSlip, {}),
		BoundaryConditionD3Raw(BoundaryConditionType::Mixed, {283, 0.01, 0.0264}) // [K], [W/(m^2 * K)] (convection coeff), thermal conductivity (gamma)
	};
}
void HeatTransferProblemD3::initBoundaryPatches(
	const std::vector<MesherBoundaryCondition>& mesherBC,
	const std::vector<MesherBoundaryConditionRaw>& mesherBCDefault)
{
	int mesherBCIndex = 0;

	std::vector<std::vector<ProblemBoundaryPatch>> boundaryPatches(2);

	for (int i = 0; i < 2; i++) {

		boundaryPatches[i].reserve(boundaryConditions[i].size() + 1);

		for (int j = 0; j < boundaryConditions[i].size(); j++) {
			boundaryPatches[i].emplace_back(
				boundaryConditions[i][j].type,
				mesherBC[mesherBCIndex].faceIDs,
				boundaryConditions[i][j].values
			);
			mesherBCIndex++;
		}

		boundaryPatches[i].emplace_back(
			defaultBoundaryConditions[i].type,
			mesherBCDefault[i].faceIDs,
			defaultBoundaryConditions[i].values
		);
	}

	fields.velocity.initBoundaryPatches(boundaryPatches[0]);
	fields.temperature.initBoundaryPatches(boundaryPatches[1]);
};

void HeatTransferProblemD3::initFields(const Mesh<MeshDim::D3>& mesh)
{
	fields.velocity.initFiled(mesh.getElements()->cells);
	fields.temperature.initFiled(mesh.getElements()->cells);
	fields.gradTemperature.initFiled(mesh.getElements()->cells);
	fields.massFlowRate.initFiled(mesh.getElements()->faces);
	fields.gradVelocity.initFiled(mesh.getElements()->cells);
	fields.pressure.initFiled(mesh.getElements()->cells);
	fields.gradPressure.initFiled(mesh.getElements()->cells);
}

void HeatTransferProblemD3::addVelocityBoundaryCondition(
	const BoundaryConditionD3& bc)
{
	boundaryConditions[0].push_back(bc);
};

void HeatTransferProblemD3::addTemperatureBoundaryCondition(
	const BoundaryConditionD3& bc)
{
	boundaryConditions[1].push_back(bc);
}

void HeatTransferProblemD3::addVelocityInitialCondition(
	const Vector<GeometryDim::D3>& ic)
{
	fields.velocity.initialObj = ic;
};

void HeatTransferProblemD3::addTemperatureInitialCondition(
	const double& ic)
{
	fields.temperature.initialObj = ic;
};