#include <visuals/displayer/scenes/vectorFieldScene.h>

VectorFieldScene::VectorFieldScene(
	const std::string& fieldName,
	vtkSmartPointer<vtkUnstructuredGrid> vtkGrid,
	const Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& field
) : 
	_fieldName(fieldName),
	_vtkGrid(vtkGrid), 
	_field(field),
	_vectors(vtkNew<vtkDoubleArray>()),
	_mask(vtkNew<vtkMaskPoints>()),
	_lut(vtkNew<vtkLookupTable>()),
	_mapper(vtkNew<vtkPolyDataMapper>())
{
	_vectors->SetName(_fieldName.data());
	_vectors->SetNumberOfComponents(3);
	_vectors->SetNumberOfTuples(_vtkGrid->GetNumberOfCells());

	double magMax = 0;
	double magMin = field.values[0].getMagnitude();

	updateScene();

	_vtkGrid->GetCellData()->AddArray(_vectors);
	activateScene();

	vtkNew<vtkCellCenters> cellCenters;
	cellCenters->SetInputData(_vtkGrid);
	cellCenters->Update();

	/*vtkSmartPointer<vtkPoints> centroids = cellCenters->GetOutput()->GetPoints();
	for (int i = 0; i < _field.values.length; i++) {
		double cords[3];
		centroids->GetPoint(i, cords);
		std::cout << "VTK: [ " << cords[0] << ", " << cords[1] << ", " << cords[2] << " ]\n";
	}*/

	cellCenters->GetOutput()->GetPointData()->SetActiveVectors(_fieldName.data());

	_mask->SetInputConnection(cellCenters->GetOutputPort());
	_mask->SetMaximumNumberOfPoints(_amountOfVectors);
	_mask->RandomModeOn();
	_mask->SetRandomModeType(2);
	_mask->Update();

	vtkNew<vtkArrowSource> arrowSource;

	vtkNew<vtkGlyph3D> glyph;
	glyph->SetSourceConnection(arrowSource->GetOutputPort());
	glyph->SetInputConnection(_mask->GetOutputPort());
	glyph->SetVectorModeToUseVector();
	glyph->OrientOn();
	glyph->SetScaleModeToDataScalingOff();
	glyph->SetScaleFactor(0.2);
	glyph->SetColorModeToColorByVector();
	glyph->Update();

	_lut->SetHueRange(0.667, 0.0);
	_lut->SetSaturationRange(1, 1);
	_lut->SetVectorModeToMagnitude();
	//_lut->SetTableRange(magMin, magMax);
	_lut->Build();

	_mapper->SetInputConnection(glyph->GetOutputPort());
	_mapper->SetLookupTable(_lut);
	_mapper->SelectColorArray(_fieldName.data());
	_mapper->SetScalarModeToUsePointData();
	_mapper->SelectColorArray(_fieldName.data());
	//_mapper->SetScalarRange(magMin, magMax);

	vtkNew<vtkScalarBarActor> scalarBar;
	scalarBar->SetLookupTable(_lut);
	scalarBar->SetTitle(_fieldName.data());
	scalarBar->SetNumberOfLabels(8);
	scalarBar->SetPosition(0.93, 0);
	scalarBar->SetHeight(0.7);
	scalarBar->SetWidth(0.07);

	vtkNew<vtkActor> glyphActor;
	glyphActor->SetMapper(_mapper);

	actors.push_back(glyphActor);
	viewProps.push_back(scalarBar);
}

void VectorFieldScene::handleKeyPress(const std::string& key) {

	auto setAmount = [&](int nextAmount) {
		_amountOfVectors = nextAmount;
		_mask->SetMaximumNumberOfPoints(nextAmount);
		_mask->Modified();
	};

	if (key == "Up") {
		int nextAmount = _amountOfVectors + _amountStep;
		if (nextAmount > _field.values.length) { return; }

		setAmount(nextAmount);
	}
	else if (key == "Down") {
		int nextAmount = _amountOfVectors - _amountStep;
		if (nextAmount < 0) { return; }

		setAmount(nextAmount);
	}
};

void VectorFieldScene::activateScene() {
	_vtkGrid->GetCellData()->SetActiveVectors(_fieldName.data());
}

void VectorFieldScene::updateScene()
{
	double magMax = 0;
	double magMin = _field.values[0].getMagnitude();

	for (vtkIdType i = 0; i < _vtkGrid->GetNumberOfCells(); ++i)
	{
		_vectors->SetTuple3(
			i,
			_field.values[i].comp[0],
			_field.values[i].comp[1],
			_field.values[i].comp[2]);
		double mag = _field.values[i].getMagnitude();
		if (mag > magMax) {
			magMax = mag;
		}
		else if (mag < magMin) {
			magMin = mag;
		}
	}

	_vectors->Modified();
	_lut->SetRange(magMin, magMax);
	_lut->Build();
	_mapper->SetScalarRange(magMin, magMax);
};

