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
#include <string>

#include <problem/field.h>
#include <mesh/meshElements/meshElements.h>

class Scene {
public:
	std::vector<vtkSmartPointer<vtkActor>> actors;

	virtual void handleKeyPress(const std::string& key) = 0;
};

class ScalarFieldScene : public Scene {
public:
	void handleKeyPress(const std::string& key) override;
	
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

	std::string _fieldName;
	const Field<double, Cell<MeshDim::D3>>& _field;

	int _currentPlaneOrientation = 2;
	double _currentPlanePos[3];
	double _stepSize = 0.125;

	void _orientPlane();
};