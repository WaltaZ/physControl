#pragma once

#include <simulation/discretization/gradient/gradient.h>
#include <utility/cudaUtils.h>
#include <utility/utility.h>
#include "gradientGaussKernel.h"

// TODO: Change to GradObj

class GradientGauss : public Gradient<GradientGauss> {	
public:
	template<typename Obj, typename GradObj>
	void computeImpl(
		Field<Obj, Cell<MeshDim::D3>>& field,
		Field<GradObj, Cell<MeshDim::D3>>& destField,
		Mesh<MeshDim::D3>& mesh
	);
};