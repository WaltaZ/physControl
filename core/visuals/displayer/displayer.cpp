#include <visuals/displayer/displayer.h>

#include <visuals/displayer/customInteractorStyle.h>
#include <visuals/displayer/scenes/vectorFieldScene.h>

// --------------------- DISPLAYER -----------------------------

Displayer::Displayer(
	const Mesh<MeshDim::D3>& mesh)
	:
	_renderer(vtkNew<vtkRenderer>()),
	_window(vtkNew<vtkRenderWindow>()),
	_interactor(vtkNew<vtkRenderWindowInteractor>()),
	_cow(vtkNew<vtkCameraOrientationWidget>()),
	_vtkGrid(vtkAdapter::createVtkMeshD3(mesh))
{
	vtkObject::GlobalWarningDisplayOff();
	_renderer->SetBackground(0.1, 0.1, 0.1);

	_window->AddRenderer(_renderer);
	_window->SetSize(1200, 800);

	_interactor->SetRenderWindow(_window);

	_cow->SetParentRenderer(_renderer);
	_cow->SetInteractor(_interactor);
	_cow->On();
}

void Displayer::display() {
	_window->Render();
	_interactor->Start();
}

// ----------------------------- SCENE DISPLAYER ------------------------------

void SceneDisplayer::_addBaseActors()
{
	for (const auto& actor : _baseActors) {
		_renderer->AddActor(actor);
	}
}

SceneDisplayer::SceneDisplayer(const Mesh<MeshDim::D3>& mesh) : Displayer(mesh) {
	vtkNew<SceneDisplayerCustomInteractorStyle> interactorStyle;
	interactorStyle->displayer = this;
	_interactor->SetInteractorStyle(interactorStyle);
};

void SceneDisplayer::displayScene(int index) {
	
	_renderer->RemoveAllViewProps();
	
	_addBaseActors();

	for (const auto& actor : _scenes[index]->actors) {
		_renderer->AddActor(actor);
	}
	for (const auto& viewProp : _scenes[index]->viewProps) {
		_renderer->AddViewProp(viewProp);
	}
	_sceneIndex = index;

	_scenes[index]->activateScene();
	_window->Render();
}

bool SceneDisplayer::_switchScenesHandler(const std::string& key)
{
	if (key == "Right") {
		int nextScene = _sceneIndex + 1;
		if (nextScene >= _scenes.size()) { return true; }
		displayScene(nextScene);
		return true;
	}
	else if (key == "Left") {
		int previousScene = _sceneIndex - 1;
		if (previousScene < 0) { return true; }
		_sceneIndex = previousScene;
		displayScene(previousScene);
		return true;
	}
	return false;
}

// ----------------------------- FIELD DISPLAYER -----------------------------

FieldDisplayer::FieldDisplayer(
	HeatTransferSimulationD3& simulation) : SceneDisplayer(simulation.getMesh()), _simulation(simulation)
{
	HeatTransferProblemD3& problem = simulation.getProblem();

	_scenes.push_back(std::make_unique<ScalarFieldScene>(
		"Temperature",
		_vtkGrid,
		problem.fields.temperature
	));
	_scenes.push_back(std::make_unique<VectorFieldScene>(
		"GradTemp",
		_vtkGrid,
		problem.fields.gradTemperature
	));

	_scenes.push_back(std::make_unique<VectorFieldScene>(
		"Velocity",
		_vtkGrid,
		problem.fields.velocity
	));

	_scenes.push_back(std::make_unique<ScalarFieldScene>(
		"Pressure",
		_vtkGrid,
		problem.fields.pressure
	));

	_scenes.push_back(std::make_unique<VectorFieldScene>(
		"GradPressure",
		_vtkGrid,
		problem.fields.gradPressure
	));

	for (const auto& bcOfSingleField : problem.boundaryConditions) {
		for (const auto& bc : bcOfSingleField) {
			_baseActors.push_back(visualsUtility::getBoundaryConditionContourActor(bc));
		}
		_baseActors.push_back(visualsUtility::getMeshContourActor(_vtkGrid));
	}

	_addBaseActors();
	displayScene(0);
}

void FieldDisplayer::handleKeyPress(const std::string& key)
{
	if (_switchScenesHandler(key)) {
	}
	else if (key == "space") {
		_simulation.nextStep();

		for (auto& scene : _scenes) {
			scene->updateScene();
		}
	}
	else {
		_scenes[_sceneIndex]->handleKeyPress(key);
	}
	_window->Render();
}
