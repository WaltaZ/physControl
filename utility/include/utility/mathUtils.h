#pragma once

#include <vector>
#include <geometry/vector.h>
#include <geometry/matrixTensor.h>
#include <cuda_runtime.h>

namespace mathUtils {

    using V2 = Vector<GeometryDim::D2>;
    using V3 = Vector<GeometryDim::D3>;
    using T2 = MatrixTensor<GeometryDim::D2>;
    using T3 = MatrixTensor<GeometryDim::D3>;

    std::vector<double> linearlyInterpolatePointsWithSpacing(
        const std::vector<double>& points,
        double spacing);

    template<class Obj>
    __host__ __device__
    Obj createMathObj(const double* comp); // A walkaround to fix

    template<class Obj> // Another walkaround
    __host__ __device__
    uint32_t getNumOfComp(const Obj& obj);
}