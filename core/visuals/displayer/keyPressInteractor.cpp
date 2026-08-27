#include <visuals/displayer/keyPressInteractor.h>

#include <visuals/displayer/displayer.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(SceneDisplayerKeyPressInteractorStyle);

void SceneDisplayerKeyPressInteractorStyle::OnKeyPress(){
    vtkRenderWindowInteractor* rwi = this->Interactor;
    std::string key = rwi->GetKeySym();

    displayer->handleKeyPress(key);

    // Forward events.
    vtkInteractorStyleTrackballCamera::OnKeyPress();
    };