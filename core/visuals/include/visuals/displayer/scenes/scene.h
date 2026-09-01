#pragma once

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkActor2D.h>

class Scene {
public:
	std::vector<vtkSmartPointer<vtkActor>> actors;
	std::vector<vtkSmartPointer<vtkActor2D>> viewProps;

	virtual void handleKeyPress(const std::string& key) = 0;
	virtual void activateScene() = 0;
	virtual void updateScene() = 0;
};

#include "scalarFieldScene.h"