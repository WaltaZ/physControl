#pragma once

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>
#include <algorithm>

class SceneDisplayer;

class SceneDisplayerCustomInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static SceneDisplayerCustomInteractorStyle* New();
    vtkTypeMacro(SceneDisplayerCustomInteractorStyle, vtkInteractorStyleTrackballCamera);

    SceneDisplayer* displayer = nullptr;

    virtual void OnKeyPress() override;

    void Rotate() override;

private:
    static double ComputeElevationFromUp(vtkCamera* camera)
    {
        double pos[3], focal[3];
        camera->GetPosition(pos);
        camera->GetFocalPoint(focal);

        double dir[3] = { pos[0] - focal[0], pos[1] - focal[1], pos[2] - focal[2] };
        double horizLen = std::sqrt(dir[0] * dir[0] + dir[1] * dir[1]);
        return vtkMath::DegreesFromRadians(std::atan2(dir[2], horizLen));
    }
};