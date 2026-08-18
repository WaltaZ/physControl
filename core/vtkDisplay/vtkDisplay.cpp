#include "include/vtkDisplay/vtkDisplay.h"

#include <vtkActor.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVertexGlyphFilter.h>

void displayTest(const Mesh<MeshDim::D3>& mesh) {
    // -------------------------------------------------------
    // 1. Create points
    // -------------------------------------------------------

    vtkNew<vtkPoints> points;

    for(const auto& node : mesh.nodes) {
        points->InsertNextPoint(node.pos[0], node.pos[1], node.pos[2]);
    }

    // -------------------------------------------------------
    // 2. Put points into vtkPolyData
    // -------------------------------------------------------

    vtkNew<vtkPolyData> polyData;
    polyData->SetPoints(points);


    // -------------------------------------------------------
    // 3. Convert points into visible vertices
    // -------------------------------------------------------

    vtkNew<vtkVertexGlyphFilter> vertexFilter;
    vertexFilter->SetInputData(polyData);
    vertexFilter->Update();


    // -------------------------------------------------------
    // 4. Create mapper
    // -------------------------------------------------------

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(vertexFilter->GetOutputPort());


    // -------------------------------------------------------
    // 5. Create actor
    // -------------------------------------------------------

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    actor->GetProperty()->SetPointSize(3);       // smaller
    actor->GetProperty()->SetColor(1.0, 1.0, 1.0); // white

    actor->GetProperty()->RenderPointsAsSpheresOn();

    // -------------------------------------------------------
    // 6. Create renderer
    // -------------------------------------------------------

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.1, 0.1);


    // -------------------------------------------------------
    // 7. Create render window
    // -------------------------------------------------------

    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    renderWindow->SetSize(800, 600);


    // -------------------------------------------------------
    // 8. Create interactor
    // -------------------------------------------------------

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(renderWindow);


    // -------------------------------------------------------
    // 9. Display
    // -------------------------------------------------------

    renderWindow->Render();
    interactor->Start();
}