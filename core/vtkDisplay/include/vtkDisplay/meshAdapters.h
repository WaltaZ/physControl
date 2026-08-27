#pragma once

#include <mesh/mesh.h>

#include <vtkPoints.h>
#include <vtkNew.h>
#include <vtkUnstructuredGrid.h>

namespace vtkAdapter {

	vtkSmartPointer<vtkUnstructuredGrid> createVtkMeshD3(const Mesh<MeshDim::D3>& mesh);

}