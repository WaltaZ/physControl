#pragma once

#include <visuals/adapters.h>
#include <visuals/displayer/scenes/scene.h>
#include <visuals/displayer/customInteractorStyle.h>
#include <mesh/mesh.h>

#include <vtkActor.h>
#include <vtkActorCollection.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkCameraOrientationWidget.h>
#include <vtkAxesActor.h>
#include <vtkNew.h>

#include <vector>
#include <memory>
#include <simulation/simulation.h>

class Displayer {
protected:
	vtkSmartPointer<vtkRenderer> _renderer;
	vtkSmartPointer<vtkRenderWindow> _window;
	vtkSmartPointer<vtkRenderWindowInteractor> _interactor;
	vtkSmartPointer<vtkCameraOrientationWidget> _cow;
	vtkSmartPointer<vtkUnstructuredGrid> _vtkGrid;
public:
	Displayer(const Mesh<MeshDim::D3>& mesh);
	void display();
};

class SceneDisplayer : public Displayer {
protected:
	std::vector<std::unique_ptr<Scene>> _scenes;
	std::vector<vtkSmartPointer<vtkActor>> _baseActors;
	int _sceneIndex = 0;

	void _addBaseActors();
	bool _switchScenesHandler(const std::string& key);
public:
	SceneDisplayer(const Mesh<MeshDim::D3>& mesh);

	virtual void handleKeyPress(const std::string& key) = 0;
	void displayScene(int index);
};

class FieldDisplayer : public SceneDisplayer {	
protected:
	HeatTransferSimulationD3& _simulation;
public:
	FieldDisplayer(HeatTransferSimulationD3& simulation);
	void handleKeyPress(const std::string& key) override;
};