#pragma once

#include <vector>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkPlane.h>
#include <vtkDoubleArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellData.h>
#include <vtkCutter.h>
#include <vtkLookupTable.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkOutlineFilter.h>
#include <vtkScalarBarActor.h>
#include <string>

#include <problem/field.h>
#include <mesh/meshElements/meshElements.h>
#include <visuals/displayer/scenes/scene.h>
#include <visuals/visualsUtility.h>

class ScalarFieldScene : public Scene {
public:
	void handleKeyPress(const std::string& key) override;

	void activateScene() override;

	void updateScene() override;

	ScalarFieldScene(
		const std::string& fieldName,
		vtkSmartPointer<vtkUnstructuredGrid> vtkGrid,
		const Field<double, Cell<MeshDim::D3>>& field
	);

private:
	vtkSmartPointer<vtkUnstructuredGrid> _vtkGrid;
	double _vtkGridBounds[6];

	vtkSmartPointer<vtkPlane> _vtkPlane;
	vtkSmartPointer<vtkDoubleArray> _vtkFieldValues;
	vtkSmartPointer<vtkLookupTable> _lut;
	vtkSmartPointer<vtkPolyDataMapper> _mapper;

	std::string _fieldName;
	const Field<double, Cell<MeshDim::D3>>& _field;

	int _currentPlaneOrientation = 2;
	double _currentPlanePos[3];
	double _stepSize = 0.125;

	void _orientPlane();
	void _setUpPlaneActor();
};