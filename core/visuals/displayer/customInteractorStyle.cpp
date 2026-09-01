#include <visuals/displayer/customInteractorStyle.h>

#include <visuals/displayer/displayer.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(SceneDisplayerCustomInteractorStyle);

void SceneDisplayerCustomInteractorStyle::OnKeyPress(){
    vtkRenderWindowInteractor* rwi = this->Interactor;
    std::string key = rwi->GetKeySym();

    displayer->handleKeyPress(key);

    vtkInteractorStyleTrackballCamera::OnKeyPress();
};

void SceneDisplayerCustomInteractorStyle::Rotate() {

    if (!this->CurrentRenderer) return;

    vtkRenderWindowInteractor* rwi = this->Interactor;

    int dx = rwi->GetEventPosition()[0] - rwi->GetLastEventPosition()[0];
    int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];

    int* size = this->CurrentRenderer->GetRenderWindow()->GetSize();

    constexpr double speedCoeff = 0.15;

    double azimuthSpeed = speedCoeff * 180.0 / size[0];
    double elevationSpeed = speedCoeff * 180.0 / size[1];

    double azimuth = -dx * azimuthSpeed * this->MotionFactor;
    double elevationDelta = -dy * elevationSpeed * this->MotionFactor;

    vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();

    camera->Azimuth(azimuth);

    double currentElevation = ComputeElevationFromUp(camera);
    double newElevation = currentElevation + elevationDelta;

    const double kMaxElevation = 89.0;
    newElevation = std::clamp(newElevation, -kMaxElevation, kMaxElevation);
    double clampedDelta = newElevation - currentElevation;

    camera->Elevation(clampedDelta);

    camera->SetViewUp(0.0, 0.0, 1.0);

    this->CurrentRenderer->ResetCameraClippingRange();
    rwi->Render();
}