#pragma once

#include <simulation/discretization/gradient/gradient.h>
#include <utility/cudaUtils.h>
#include <utility/utility.h>
#include "gradientGaussKernel.h"

class GradientGauss : public Gradient<MeshDim::D3> {	
public:
	void compute(
		Field<double, Cell<MeshDim::D3>>& field,
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
		Mesh<MeshDim::D3>& mesh
	) override;

	void compute(
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>>& field,
		Field<MatrixTensor<GeometryDim::D3>, Cell<MeshDim::D3>>& destField,
		Mesh<MeshDim::D3>& mesh
	) override;
};