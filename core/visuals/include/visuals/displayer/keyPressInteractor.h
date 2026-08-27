#pragma once

#include <vtkInteractorStyleTrackballCamera.h>

class SceneDisplayer;

class SceneDisplayerKeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static SceneDisplayerKeyPressInteractorStyle* New();
    vtkTypeMacro(SceneDisplayerKeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);

    SceneDisplayer* displayer = nullptr;

    virtual void OnKeyPress() override;
};