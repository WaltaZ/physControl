#pragma once

#include <simulation/discretization/gradient/gradient.h>
#include <utility/cudaUtils.h>
#include <utility/utility.h>
#include "gradientGaussKernel.h"

class GradientGauss : public Gradient<GradientGauss> {	
public:
	template<typename Obj, typename ObjDest>
	void computeImpl(
		Field<Obj, Cell<MeshDim::D3>>& field,
		Field<ObjDest, Cell<MeshDim::D3>>& destField,
		Mesh<MeshDim::D3>& mesh
	);
};