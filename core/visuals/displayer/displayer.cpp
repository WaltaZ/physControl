#include <visuals/displayer/displayer.h>

#include <visuals/displayer/keyPressInteractor.h>

// --------------------- DISPLAYER -----------------------------

Displayer::Displayer(
        const Mesh<MeshDim::D3>& mesh) 
    :
        _renderer(vtkNew<vtkRenderer>()),
        _window(vtkNew<vtkRenderWindow>()),
        _interactor(vtkNew<vtkRenderWindowInteractor>()),
        _vtkMesh(vtkAdapter::createVtkMeshD3(mesh))
{
    _renderer->SetBackground(0.1, 0.1, 0.1);

    _window->AddRenderer(_renderer);
    _window->SetSize(800, 600);

    _interactor->SetRenderWindow(_window);
    vtkNew<vtkInteractorStyleTrackballCamera> style;
    _interactor->SetInteractorStyle(style);

    vtkNew<vtkCameraOrientationWidget> cow;
    cow->SetParentRenderer(_renderer);
    cow->On();
}

void Displayer::display() {
    _window->Render();
    _interactor->Start();
}

// ----------------------------- SCENE DISPLAYER ------------------------------

SceneDisplayer::SceneDisplayer(const Mesh<MeshDim::D3>& mesh) : Displayer(mesh) {

    vtkNew<SceneDisplayerKeyPressInteractorStyle> interactorStyle;

    interactorStyle->displayer = this;

    _interactor->SetInteractorStyle(interactorStyle);
};

void SceneDisplayer::displayScene(int index) {
    _renderer->RemoveAllViewProps();
    for (auto& actor : _scenes[index]->actors) {
        _renderer->AddActor(actor);
    }
    _sceneIndex = index;
    _window->Render();
}

void SceneDisplayer::handleKeyPress(const std::string& key) {
    if (key == "Right") {
        int nextScene = _sceneIndex + 1;
        if (nextScene >= _scenes.size()) { return; }
        displayScene(nextScene);
        return;
    }
    else if (key == "Left") {
        int previousScene = _sceneIndex - 1;
        if (previousScene < 0) { return; }
        _sceneIndex = previousScene;
        displayScene(previousScene);
        return;
    }
    _scenes[_sceneIndex]->handleKeyPress(key);
    _window->Render();
}

// ----------------------------- FIELD DISPLAYER -----------------------------

FieldDisplayer::FieldDisplayer(
    const Mesh<MeshDim::D3>& mesh,
    const HeatTransferD3& problem) : SceneDisplayer(mesh)
{
    _scenes.push_back(std::make_unique<ScalarFieldScene>(
        "Temperature",
        _vtkMesh,
        problem.fields.temperature
    ));

    displayScene(0);
}