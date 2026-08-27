#include <visuals/displayer/scene.h>

ScalarFieldScene::ScalarFieldScene(
    const std::string& fieldName,
	vtkSmartPointer<vtkUnstructuredGrid> vtkGrid,
	const Field<double, Cell<MeshDim::D3>>& field
	) :
	_vtkGrid(vtkGrid),
	_field(field),
    _fieldName(fieldName)
{
    vtkNew<vtkDoubleArray> scalars;
    scalars->SetName(_fieldName.data());
    scalars->SetNumberOfComponents(1);
    scalars->SetNumberOfTuples(_vtkGrid->GetNumberOfCells());

    for (vtkIdType i = 0; i < _vtkGrid->GetNumberOfCells(); ++i)
    {
        scalars->SetValue(i, field.values[i]);
    }

    _vtkGrid->GetCellData()->SetScalars(scalars);

    _vtkGrid->GetBounds(_vtkGridBounds);

    _currentPlanePos[0] = (_vtkGridBounds[0] + _vtkGridBounds[1]) / 2;
    _currentPlanePos[1] = (_vtkGridBounds[3] + _vtkGridBounds[2]) / 2;
    _currentPlanePos[2] = (_vtkGridBounds[5] + _vtkGridBounds[4]) / 2;

    _vtkPlane = vtkNew<vtkPlane>();
    _vtkPlane->SetOrigin(_currentPlanePos);

    _orientPlane();

    vtkNew<vtkCutter> cutter;
    cutter->SetInputData(_vtkGrid);
    cutter->SetCutFunction(_vtkPlane);

    double range[2];
    _vtkGrid->GetCellData()->GetScalars()->GetRange(range);

    vtkNew<vtkLookupTable> lut;
    lut->SetHueRange(0.667, 0.0);
    lut->SetRange(range[0], range[1]);
    lut->Build();

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cutter->GetOutputPort());
    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(range);
    mapper->SetScalarModeToUseCellData();
    mapper->SelectColorArray("Test");
    mapper->ScalarVisibilityOn();

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    actors.push_back(actor);
}

void ScalarFieldScene::handleKeyPress(const std::string& key) {
    if (key == "Up") {
        double nextAxialPos = 
            _currentPlanePos[_currentPlaneOrientation] + 
            (_stepSize * (
                _vtkGridBounds[2*_currentPlaneOrientation + 1] - 
                _vtkGridBounds[2* _currentPlaneOrientation]
                )
            );

        if (nextAxialPos >= _vtkGridBounds[2 * _currentPlaneOrientation + 1]) { return; }
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

        if (nextAxialPos <= _vtkGridBounds[2 * _currentPlaneOrientation]) { return; }
        _currentPlanePos[_currentPlaneOrientation] = nextAxialPos;
        _vtkPlane->SetOrigin(_currentPlanePos);

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

void ScalarFieldScene::_orientPlane() {
    double normal[3] = { 0, 0, 0 };
    normal[_currentPlaneOrientation] = 1;
    _vtkPlane->SetNormal(normal);
};