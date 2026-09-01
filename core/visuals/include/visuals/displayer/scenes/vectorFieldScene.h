#pragma once

#include "scene.h"
#include <vtkCellCenters.h>
#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>
#include <vtkPolyDataMapper.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkMaskPoints.h>
#include <vtkScalarBarActor.h>

class VectorFieldScene : public Scene {
protected:
	std::string _fieldName;
	vtkSmartPointer<vtkDoubleArray> _vectors;
	vtkSmartPointer<vtkMaskPoints> _mask;
	vtkSmartPointer<vtkUnstructuredGrid> _vtkGrid;
	vtkSmartPointer<vtkLookupTable> _lut;
	vtkSmartPointer<vtkPolyDataMapper> _mapper;

	const Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& _field;

	int _amountOfVectors = 1000;
	int _amountStep = 100;

public:
	VectorFieldScene(
		const std::string& fieldName,
		vtkSmartPointer<vtkUnstructuredGrid> vtkGrid,
		const Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& field);

	void handleKeyPress(const std::string& key) override;
	void activateScene() override;
	void updateScene() override;
};