#include "include/problem/problem.h"

ProblemD3::ProblemD3(
	const ProblemGeometryD3& geometry) : geometry(geometry) {
};

HeatTransferD3::HeatTransferD3(
	const ProblemGeometryD3& geometry) : ProblemD3(geometry) 
{
	boundaryConditions = std::vector<std::vector<BoundaryConditionD3>>{ 2 };
	defaultBoundaryConditions = std::vector<BoundaryConditionD3>{};
};


void HeatTransferD3::addVelocityBoundaryCondition(
	const BoundaryConditionD3& bc)
{
	boundaryConditions[0].push_back(bc);
};

void HeatTransferD3::addTemperatureBoundaryCondition(
	const BoundaryConditionD3& bc)
{
	boundaryConditions[1].push_back(bc);
}

void HeatTransferD3::addVelocityInitialCondition(
	const Vector<GeometryDim::D3>& ic)
{
	fields.velocity.initialObj = ic;
};

void HeatTransferD3::addTemperatureInitialCondition(
	const double& ic)
{
	fields.temperature.initialObj = ic;
};