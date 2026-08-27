#pragma once

#include <visuals/adapters.h>
#include <visuals/displayer/scene.h>
#include <visuals/displayer/keyPressInteractor.h>
#include <mesh/mesh.h>

#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCameraOrientationWidget.h>
#include <vtkAxesActor.h>
#include <vtkNew.h>

#include <vector>
#include <memory>

class Displayer {
protected:
	vtkSmartPointer<vtkRenderer> _renderer;
	vtkSmartPointer<vtkRenderWindow> _window;
	vtkSmartPointer<vtkRenderWindowInteractor> _interactor;
	vtkSmartPointer<vtkUnstructuredGrid> _vtkMesh;
public:
	Displayer(const Mesh<MeshDim::D3>& mesh);
	void display();

	//void addMeshActor();
	//void addBoundaryActor(const std::vector<BoundaryPatch>& patches);
	//void addScalarFieldActor(const Field<double, Cell<MeshDim::D3>>& field);
};

class SceneDisplayer : public Displayer {
protected:
	std::vector<std::unique_ptr<Scene>> _scenes;
	int _sceneIndex = 0;
public:
	SceneDisplayer(const Mesh<MeshDim::D3>& mesh);

	void handleKeyPress(const std::string& key);
	void displayScene(int index);
};

class FieldDisplayer : public SceneDisplayer {	
public:
	FieldDisplayer(
		const Mesh<MeshDim::D3>& mesh, 
		const HeatTransferD3& problem);
};