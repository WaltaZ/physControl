#include <visuals/adapters.h>

#include <vtkPoints.h>
#include <vtkNew.h>
#include <vtkUnstructuredGrid.h>

namespace vtkAdapter {

	vtkSmartPointer<vtkUnstructuredGrid> createVtkMeshD3(const Mesh<MeshDim::D3>& mesh) {

		vtkNew<vtkPoints> points;

		for (int i = 0; i < mesh.getElements()->nodes.length; i++) {
			points->InsertNextPoint(
				mesh.getElements()->nodes[i].pos[0],
				mesh.getElements()->nodes[i].pos[1],
				mesh.getElements()->nodes[i].pos[2]
			);
		}

		vtkNew<vtkUnstructuredGrid> vtkMesh;

		vtkMesh->SetPoints(points);

		for (int i = 0; i < mesh.getElements()->cells.length; i++) {

			std::vector<vtkIdType> ids;

			for (int j = 0; j < mesh.getElements()->cells[i].cellNodeIDs.length; j++) {
				ids.push_back(static_cast<vtkIdType>(mesh.getElements()->cells[i].cellNodeIDs[j]));
			}

			vtkMesh->InsertNextCell(
				VTK_HEXAHEDRON,
				8,
				ids.data()
			);
		}

		return vtkMesh;
	};
}