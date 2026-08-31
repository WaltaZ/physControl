#include "include/geometry/geometryUtils.h"

namespace geomOp {

    // -------------------------------- Dot product ------------------------------------

    template<GeometryDim dim>
    double vecDotProduct(
        const Vector<dim>& vec1,
        const Vector<dim>& vec2
    )
    {
        double product = 0;

        for (int i = 0; i < geometryDimSize(dim); ++i) {
            product += vec1.comp[i] * vec2.comp[i];
        }

        return product;
    }

    template double vecDotProduct(const V2&, const V2&);
    template double vecDotProduct(const V3&, const V3&);

    // -------------------------------- Cross product ------------------------------------

    Vector<GeometryDim::D3> vecCrossProduct(const V2& vec1, const V2& vec2)
    {
        return Vector<GeometryDim::D3>({
            0,
            0,
            (vec1.comp[0] * vec2.comp[1]) - (vec1.comp[1] * vec2.comp[0])
            });
    };

    Vector<GeometryDim::D3> vecCrossProduct(const V3& vec1, const V3& vec2)
    {
        return Vector<GeometryDim::D3>({
            (vec1.comp[1] * vec2.comp[2]) - (vec1.comp[2] * vec2.comp[1]),
            (vec1.comp[2] * vec2.comp[0]) - (vec1.comp[0] * vec2.comp[2]),
            (vec1.comp[0] * vec2.comp[1]) - (vec1.comp[1] * vec2.comp[0])
        });
    }

    // --------------------------------- Triangle area -------------------------------------

    template<GeometryDim dim>
    V3 calculateTriangleArea(const Point<dim>& p1, const Point<dim>& p2, const Point<dim>& p3)
    {
        Vector<dim> v1(p1), v2(p2), v3(p3);

        V3 area = vecCrossProduct((v2 - v1), (v3 - v1)) * 0.5;
        return area;
    }

    template V3 calculateTriangleArea(const P2& p1, const P2& p2, const P2& p3);
    template V3 calculateTriangleArea(const P3& p1, const P3& p2, const P3& p3);

    // ----------------------------- Move point with vector --------------------------------

    template<GeometryDim dim>
    Point<dim> movePointWithVector(const Point<dim>& point, const Vector<dim>& vector)
    {

        Point<dim> resultPoint = Point<dim>();

        for (int i = 0; i < geometryDimSize(dim); i++) {
            resultPoint.pos[i] = point.pos[i] + vector.comp[i];
        };

        return resultPoint;
    }

    template P2 movePointWithVector(const P2& point, const V2& vector);
    template P3 movePointWithVector(const P3& point, const V3& vector);

    // ----------------------------- Calculating Geocenter ---------------------------------

    template<GeometryDim dim>
    Point<dim> calculateGeoCenter(const std::vector<Point<dim>*>& points)
    {
        int numOfPoints = points.size();
        Point<dim> _geoCenter = Point<dim>();
        for (int i = 0; i < numOfPoints; i++) {
            _geoCenter = _geoCenter + *points[i];
        };
        return _geoCenter / (double)numOfPoints;
    }

    template P2 calculateGeoCenter(const std::vector<P2*>& points);
    template P3 calculateGeoCenter(const std::vector<P3*>& points);

    // ------------------------ Linearity and coplanarity check ----------------------------

    template<GeometryDim dim>
    bool arePointsColinear(const std::vector<Point<dim>*>& points)
    {
        assert(points.size() >= 3);

        // TODO: change that and below
        constexpr double tolerance = 0.001;

        bool areColinear = true;
        for (int i = 0; i < points.size() - 2; i++) {
            Vector<dim> vecA(*points[i], *points[i + 1]);
            Vector<dim> vecB(*points[i + 1], *points[i + 2]);
            V3 result = geomOp::vecCrossProduct(vecA, vecB);
            double mag = result.getMagnitude();
            areColinear = (abs(mag) < tolerance);
            if (!areColinear) { break; };
        };
        return areColinear;
    }

    template bool arePointsColinear(const std::vector<P2*>& points);
    template bool arePointsColinear(const std::vector<P3*>& points);

    bool arePointsCoplanar(const std::vector<P3*>& points)
    {
        assert(points.size() >= 4);

        constexpr double tolerance = 0.00001;
        bool foundPlane = false;

        V3 baseNVector = V3();
        int basePointIndex = 0;

        for (int i = 0; i < points.size() - 2; i++) {
            for (int j = i + 1; j < points.size() - 1; j++) {
                for (int k = j + 1; k < points.size(); k++) {
                    bool areColinear = geomOp::arePointsColinear<GeometryDim::D3>({ points[i], points[j], points[k] });
                    if (!areColinear) {
                        foundPlane = true;
                        baseNVector = geomOp::vecCrossProduct(V3(*points[i], *points[j]), V3(*points[i], *points[k])).getNormal();
                        basePointIndex = i;
                        break;
                    }
                }
                if (foundPlane) { break; }
            };
            if (foundPlane) { break; }
        };

        if (!foundPlane) { return true; }

        for (int i = 1; i < points.size(); i++) {
            if (i != basePointIndex) {
                V3 v(*points[basePointIndex], *points[i]);
                double dot = geomOp::vecDotProduct(baseNVector, v);
                if (abs(dot) > tolerance) {
                    return false;
                }
            }

        }
        return true;
    }
}

namespace geomUtils {
    template<typename T>
    __host__ __device__
    CudaArray<double> getComponents(T* obj) {
        return CudaArray<double>(nullptr, 0, 0);
    };

    template<>
    __host__ __device__
    CudaArray<double> getComponents(double* obj) {
        return CudaArray<double>(obj, 0, 1);
    };

    template<>
    __host__ __device__
    CudaArray<double> getComponents(Vector<GeometryDim::D3>* obj) {
        return CudaArray<double>(obj->comp.data(), 0, 3);
    };

    template<>
    __host__ __device__
    CudaArray<double> getComponents(MatrixTensor<GeometryDim::D3>* obj) {
        return CudaArray<double>(obj->comp.data(), 0, 9);
    };
}

namespace geomPrint {
    void printP(const Point<GeometryDim::D2>& point) 
    {
        std::cout << "( " << point.pos[0] << ", " << point.pos[1] << " )\n";
    }

    void printV(const Vector<GeometryDim::D2>& point)
    {
        std::cout << "[ " << point.comp[0] << ", " << point.comp[1] << " ]\n";
    }

    void printP(const Point<GeometryDim::D3>& point) {
        std::cout << "( " << point.pos[0] << ", " << point.pos[1] << ", " << point.pos[2] << " )\n";
    }

    void printV(const Vector<GeometryDim::D3>& vector) {
        std::cout << "[ " << vector.comp[0] << ", " << vector.comp[1] << ", " << vector.comp[2] << " ]\n";
    }
}