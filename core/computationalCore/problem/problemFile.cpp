#include <problem/problem.h>

void HeatTransferProblemD3::saveBoundaryPatches(const std::string& path)
{
	fields->velocity->saveBoundaryPatches(path);
	fields->temperature->saveBoundaryPatches(path);
	fields->pressure->saveBoundaryPatches(path);
}

void HeatTransferProblemD3::loadBoundaryPatches(const std::string& path)
{
	fields->velocity->loadBoundaryPatches(path);
	fields->temperature->loadBoundaryPatches(path);
	fields->pressure->loadBoundaryPatches(path);
}