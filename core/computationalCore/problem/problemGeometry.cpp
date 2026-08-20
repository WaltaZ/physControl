#include "include/problem/problemGeometry.h"

ProblemGeometryD3::ProblemGeometryD3(
		const Volume& operationalGeometry) 
	: 
		operationalGeometry(operationalGeometry)
{}

ProblemGeometryCuboid::ProblemGeometryCuboid(
		const Cuboid& cuboid)
	: 
		ProblemGeometryD3(cuboid)
{}

const Cuboid& ProblemGeometryCuboid::getCuboid() const
{
	return static_cast<const Cuboid&>(operationalGeometry);
};