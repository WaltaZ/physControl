#include "include/vtkDisplay/vtkDisplay.h"

#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCellData.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

void displayMesh(const Mesh<MeshDim::D3>& mesh, const BoundaryPatch& bp)
{
    vtkSmartPointer<vtkUnstructuredGrid> vtkMesh = vtkAdapter::createVtkMeshD3(mesh);

    // Build a per-cell color array
    vtkNew<vtkUnsignedCharArray> cellColors;
    cellColors->SetNumberOfComponents(3);
    cellColors->SetName("Colors");

    vtkIdType numCells = vtkMesh->GetNumberOfCells();
    cellColors->SetNumberOfTuples(numCells);

    unsigned char defaultColor[3] = { 200, 200, 200 };
    unsigned char highlightColor[3] = { 255, 0, 0 };

    for (vtkIdType i = 0; i < numCells; ++i)
    {
        cellColors->SetTypedTuple(i, defaultColor);
    }

    std::vector<vtkIdType> facesToHighlight = {};

    // Set of cell IDs you want to highlight
    for (int i = 0; i < bp.faceIDs.length; i++) {
        facesToHighlight.push_back(static_cast<vtkIdType>(mesh.faces.data[bp.faceIDs[i]].ownerCellID));
    };

    for (vtkIdType id : facesToHighlight)
    {
        cellColors->SetTypedTuple(id, highlightColor);
    }

    vtkMesh->GetCellData()->SetScalars(cellColors);

    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(vtkMesh);

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    //actor->GetProperty()->SetRepresentationToWireframe();
    //actor->GetProperty()->EdgeVisibilityOn();
    //actor->GetProperty()->SetEdgeColor(1.0, 0.0, 0.0);
    //actor->GetProperty()->SetLineWidth(2.0);

    actor->GetProperty()->SetRepresentationToSurface();
    actor->GetProperty()->EdgeVisibilityOn();
    actor->GetProperty()->SetLineWidth(1.0);

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.1, 0.1);

    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);

    vtkNew<vtkRenderWindowInteractor> windowInteractor;
    windowInteractor->SetRenderWindow(renderWindow);

    renderWindow->Render();
    windowInteractor->Start();
}
