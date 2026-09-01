#include <visuals/visualsUtility.h>

namespace visualsUtility {
	vtkSmartPointer<vtkActor> getMeshContourActor(vtkSmartPointer<vtkUnstructuredGrid> grid) {

		vtkNew<vtkOutlineFilter> contour;
		contour->SetInputData(grid);
		contour->Update();

		vtkNew<vtkPolyDataMapper> mapper;
		mapper->SetInputConnection(contour->GetOutputPort());

		vtkNew<vtkActor> actor;
		actor->SetMapper(mapper);

		return actor;
	}

	vtkSmartPointer<vtkActor> getBoundaryConditionContourActor(const BoundaryConditionD3& bc) {

		vtkNew<vtkPoints> points;
		for (const auto& p : bc.geometry.vertices) {
			points->InsertNextPoint(
				p->pos[0],
				p->pos[1],
				p->pos[2]
			);
		}

		vtkNew<vtkPolygon> polygon;

		polygon->GetPointIds()->SetNumberOfIds(bc.geometry.vertices.size());
		for (int i = 0; i < points->GetNumberOfPoints(); i++) {
			polygon->GetPointIds()->SetId(i, i);
		}

		vtkNew<vtkCellArray> cell;
		cell->InsertNextCell(polygon);

		vtkNew<vtkPolyData> polyData;
		polyData->SetPoints(points);
		polyData->SetPolys(cell);

		vtkNew<vtkFeatureEdges> fe;

		fe->SetInputData(polyData);
		fe->BoundaryEdgesOn();
		fe->FeatureEdgesOff();
		fe->NonManifoldEdgesOff();
		fe->ManifoldEdgesOff();
		fe->Update();

		vtkNew<vtkPolyDataMapper> mapper;
		mapper->SetInputConnection(fe->GetOutputPort());

		vtkNew<vtkActor> actor;
		actor->SetMapper(mapper);

		return actor;
	};
}