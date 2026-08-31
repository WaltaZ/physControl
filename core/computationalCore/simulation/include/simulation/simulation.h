#pragma once

#include <mesh/meshElements/meshElements.h>
#include <problem/problem.h>

#include "discretization/gradient/simpleGradient.h"

class HeatTransferSimulationD3 {

	HeatTransferProblemD3& _problem;
	Mesh<MeshDim::D3>& _mesh;

public:
	HeatTransferSimulationD3(
		HeatTransferProblemD3& problem,
		Mesh<MeshDim::D3>& mesh
	);

	HeatTransferProblemD3& getProblem();
	Mesh<MeshDim::D3>& getMesh();

	void nextStep();
};