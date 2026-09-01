#include <visuals/displayer/scenes/scalarFieldScene.h>

ScalarFieldScene::ScalarFieldScene(
    const std::string& fieldName,
    vtkSmartPointer<vtkUnstructuredGrid> vtkGrid,
    const Field<double, Cell<MeshDim::D3>>& field
) :
    _vtkGrid(vtkGrid),
    _field(field),
    _fieldName(fieldName),
    _vtkFieldValues(vtkNew<vtkDoubleArray>()),
    _vtkPlane(vtkNew<vtkPlane>()),
    _lut(vtkNew<vtkLookupTable>()),
    _mapper(vtkNew<vtkPolyDataMapper>())
{
    _setUpPlaneActor();
}

void ScalarFieldScene::_setUpPlaneActor() {

    _vtkFieldValues->SetName(_fieldName.data());
    _vtkFieldValues->SetNumberOfComponents(1);
    _vtkFieldValues->SetNumberOfTuples(_vtkGrid->GetNumberOfCells());

    updateScene();

    _vtkGrid->GetCellData()->AddArray(_vtkFieldValues);
    _vtkGrid->GetCellData()->SetActiveScalars(_fieldName.data());

    _vtkGrid->GetBounds(_vtkGridBounds);

    _currentPlanePos[0] = (_vtkGridBounds[0] + _vtkGridBounds[1]) / 2;
    _currentPlanePos[1] = (_vtkGridBounds[3] + _vtkGridBounds[2]) / 2;
    _currentPlanePos[2] = (_vtkGridBounds[5] + _vtkGridBounds[4]) / 2;

    _vtkPlane->SetOrigin(_currentPlanePos);

    _orientPlane();

    vtkNew<vtkCutter> cutter;
    cutter->SetInputData(_vtkGrid);
    cutter->SetCutFunction(_vtkPlane);


    _lut->SetHueRange(0.667, 0.0);
    _lut->Build();

    _mapper->SetInputConnection(cutter->GetOutputPort());
    _mapper->SetLookupTable(_lut);
    _mapper->SetScalarModeToUseCellData();
    _mapper->SelectColorArray(_fieldName.data());
    _mapper->ScalarVisibilityOn();

    vtkNew<vtkScalarBarActor> scalarBar;
    scalarBar->SetLookupTable(_lut);
    scalarBar->SetTitle(_fieldName.data());
    scalarBar->SetNumberOfLabels(8);
    scalarBar->SetPosition(0.93, 0);
    scalarBar->SetHeight(0.7);
    scalarBar->SetWidth(0.07);

    vtkNew<vtkActor> actor;
    actor->SetMapper(_mapper);

    actors.push_back(actor);
    viewProps.push_back(scalarBar);
}

// -------------------------- EVENT HANDLER ------------------------------

void ScalarFieldScene::handleKeyPress(const std::string& key) {
    if (key == "Up") {
        double nextAxialPos =
            _currentPlanePos[_currentPlaneOrientation] +
            (_stepSize * (
                _vtkGridBounds[2 * _currentPlaneOrientation + 1] -
                _vtkGridBounds[2 * _currentPlaneOrientation]
                )
                );

        if (nextAxialPos > _vtkGridBounds[2 * _currentPlaneOrientation + 1]) { return; }
        _currentPlanePos[_currentPlaneOrientation] = nextAxialPos;
        _vtkPlane->SetOrigin(_currentPlanePos);
    }
    else if (key == "Down") {
        double nextAxialPos =
            _currentPlanePos[_currentPlaneOrientation] -
            (_stepSize * (
                _vtkGridBounds[2 * _currentPlaneOrientation + 1] -
                _vtkGridBounds[2 * _currentPlaneOrientation]
                )
                );

        if (nextAxialPos < _vtkGridBounds[2 * _currentPlaneOrientation]) { return; }
        _currentPlanePos[_currentPlaneOrientation] = nextAxialPos;
        _vtkPlane->SetOrigin(_currentPlanePos);

        if (nextAxialPos == _vtkGridBounds[2 * _currentPlaneOrientation]) {
            double temp[3];
            std::copy(_currentPlanePos, _currentPlanePos + 3, temp);
            temp[_currentPlaneOrientation] += 10e-10;
            _vtkPlane->SetOrigin(temp);
        }

    }
    else if (key == "Prior") {
        double nextStep = _stepSize * 2;
        if (nextStep > 0.5) { return; }
        _stepSize = nextStep;
    }
    else if (key == "Next") {
        _stepSize = _stepSize / 2;
    }
    else if (key == "1") {
        _currentPlaneOrientation = 0;
        _orientPlane();
    }
    else if (key == "2") {
        _currentPlaneOrientation = 1;
        _orientPlane();
    }
    else if (key == "3") {
        _currentPlaneOrientation = 2;
        _orientPlane();
    }
};

void ScalarFieldScene::activateScene() {
    _vtkGrid->GetCellData()->SetActiveScalars(_fieldName.data());
}

void ScalarFieldScene::updateScene()
{
    for (vtkIdType i = 0; i < _vtkGrid->GetNumberOfCells(); ++i)
    {
        _vtkFieldValues->SetValue(i, _field.values[i]);
    }
    _vtkFieldValues->Modified();
    
    double range[2];
    _vtkFieldValues->GetRange(range);
    _lut->SetRange(range);
    _lut->Build();
    _mapper->SetScalarRange(range);
}

void ScalarFieldScene::_orientPlane() {
    double normal[3] = { 0, 0, 0 };
    normal[_currentPlaneOrientation] = 1;
    _vtkPlane->SetNormal(normal);
};