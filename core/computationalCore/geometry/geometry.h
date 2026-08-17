#pragma once

#include "geometryDependencies.h"

#include "geometryEnums.h"

class Geometry {
public:
	virtual ~Geometry() = default;
};

#include "shapes/basic/point.h"
#include "tensor.h"
#include "matrixTensor.h"
#include "vector.h"
#include "geometryUtils.h"
#include "shapes/basic/line.h"
#include "shapes/basic/surface.h"
#include "shapes/basic/volume.h"
#include "shapes/cuboid.h"

// TESTING
#include "tests.h"