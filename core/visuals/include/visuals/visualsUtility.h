#pragma once

#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkActor.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkOutlineSource.h>
#include <vtkPolygon.h>
#include <vtkFeatureEdges.h>

#include <geometry/vector.h>
#include <problem/boundaryCondition.h>

namespace visualsUtility {
	vtkSmartPointer<vtkActor> getMeshContourActor(vtkSmartPointer<vtkUnstructuredGrid> grid);
	vtkSmartPointer<vtkActor> getBoundaryConditionContourActor(const BoundaryConditionD3& bc);
}