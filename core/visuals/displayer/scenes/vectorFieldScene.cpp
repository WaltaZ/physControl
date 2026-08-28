#include <visuals/displayer/scenes/vectorFieldScene.h>

VectorFieldScene::VectorFieldScene(
	const std::string& fieldName,
	vtkSmartPointer<vtkUnstructuredGrid> vtkGrid,
	const Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& field
) : _fieldName(fieldName), _vtkGrid(vtkGrid), _field(field)
{
	vtkNew<vtkDoubleArray> vectors;
	vectors->SetName(_fieldName.data());
	vectors->SetNumberOfComponents(3);
	vectors->SetNumberOfTuples(_vtkGrid->GetNumberOfCells());

	double magMax = 0;
	double magMin = field.values[0].getMagnitude();

	for (vtkIdType i = 0; i < _vtkGrid->GetNumberOfCells(); ++i)
	{
		vectors->SetTuple3(
			i,
			field.values[i].comp[0],
			field.values[i].comp[1],
			field.values[i].comp[2]);
		double mag = field.values[i].getMagnitude();
		if (mag > magMax) {
			magMax = mag;
		}
		else if (mag < magMin) {
			magMin = mag;
		}
	}

	_vtkGrid->GetCellData()->SetVectors(vectors);
	activateScene();

	vtkNew<vtkCellCenters> cellCenters;
	cellCenters->SetInputData(_vtkGrid);
	cellCenters->Update();

	cellCenters->GetOutput()->GetPointData()->SetActiveVectors(_fieldName.data());

	vtkNew<vtkMaskPoints> mask;
	mask->SetInputConnection(cellCenters->GetOutputPort());
	mask->SetMaximumNumberOfPoints(1000);
	mask->RandomModeOn();
	mask->SetRandomModeType(2);
	mask->Update();

	vtkNew<vtkArrowSource> arrowSource;

	vtkNew<vtkGlyph3D> glyph;
	glyph->SetSourceConnection(arrowSource->GetOutputPort());
	glyph->SetInputConnection(mask->GetOutputPort());
	glyph->SetVectorModeToUseVector();
	glyph->OrientOn();
	glyph->SetScaleModeToDataScalingOff();
	glyph->SetScaleFactor(0.2);
	glyph->SetColorModeToColorByVector();
	glyph->Update();

	double range[2];
	vectors->GetRange(range);

	vtkNew<vtkLookupTable> lut;
	lut->SetHueRange(0.667, 0.0);
	lut->SetSaturationRange(1, 1);
	lut->SetVectorModeToMagnitude();
	lut->SetTableRange(magMin, magMax);
	lut->Build();

	vtkNew<vtkPolyDataMapper> glyphMapper;
	glyphMapper->SetInputConnection(glyph->GetOutputPort());
	glyphMapper->SetLookupTable(lut);
	glyphMapper->SelectColorArray(_fieldName.data());
	glyphMapper->SetScalarModeToUsePointData();
	glyphMapper->SelectColorArray(_fieldName.data());
	glyphMapper->SetScalarRange(magMin, magMax);

	vtkNew<vtkScalarBarActor> scalarBar;
	scalarBar->SetLookupTable(lut);
	scalarBar->SetTitle(_fieldName.data());
	scalarBar->SetNumberOfLabels(8);
	scalarBar->SetPosition(0.93, 0);
	scalarBar->SetHeight(0.7);
	scalarBar->SetWidth(0.07);

	vtkNew<vtkActor> glyphActor;
	glyphActor->SetMapper(glyphMapper);

	actors.push_back(glyphActor);
	viewProps.push_back(scalarBar);
}

void VectorFieldScene::handleKeyPress(const std::string& key) {};
void VectorFieldScene::activateScene() {
	_vtkGrid->GetCellData()->SetActiveVectors(_fieldName.data());
};

