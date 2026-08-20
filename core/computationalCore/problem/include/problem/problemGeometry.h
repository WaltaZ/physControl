#pragma once

#include <geometry/geometry.h>

// --------------------------- PROBLEM GEOMETRY -------------------------------

struct ProblemGeometryD3 {

	const Volume& operationalGeometry;

	ProblemGeometryD3(
		const Volume& operationalGeometry
	);
};

// ------------------------ CUBOID PROBLEM GEOMETRY ---------------------------

struct ProblemGeometryCuboid : public ProblemGeometryD3 {
	ProblemGeometryCuboid(const Cuboid& cuboid);

	const Cuboid& getCuboid() const;
};