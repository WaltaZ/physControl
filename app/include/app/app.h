#pragma once

#include <mesh/meshElements/meshElements.h>
#include <mesh/meshers/meshers.h>

#include <problem/problem.h>
#include <problem/tests.h>

#include <simulation/simulationCore/simulation.h>
#include <simulation/discretization/discretization.h>
#include <simulation/linearSolver/jacobi/linearSolverJacobi.h>

#include <visuals/displayer/displayer.h>

#include <utility/cudaUtils.h>
#include <utility/cudaUtilsWithKernels.h>

#include <utility/debugUtils.h>

class AppSimulation {
public:
	AppSimulation() = default;
	~AppSimulation();

	void runApp();

private:
	Mesh<MeshDim::D3>* mesh;
	HeatTransferProblemD3* problem;

	void _fileProblem();
	void _newProblem();
};