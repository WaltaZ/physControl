#include "include/utility/mathUtils.h"

namespace mathUtils {
    std::vector<double> linearlyInterpolatePointsWithSpacing(const std::vector<double>& points, double spacing) {
        std::vector<double> result;
        int numOfPoints = points.size();

        for (int i = 0; i < numOfPoints - 1; i++) {
            double distance = points[i + 1] - points[i];
            double step = distance;
            int division = 1;
            while (spacing < step) {
                division++;
                step = distance / (double)division;
            }

            for (int j = 0; j < division; j++) {
                result.push_back(points[i] + (j * step));
            }
            
        }

        result.push_back(points[numOfPoints - 1]);

        return result;
    }

    // --------------------------- Create Math Obj -------------------------------

    template<class Obj>
    __host__ __device__
    Obj createMathObj(const double* comp) {
        return Obj(comp);
    }

    template  
    __host__ __device__
    V2 createMathObj(const double* comp);

    template  
    __host__ __device__
    V3 createMathObj(const double* comp);

    template  
    __host__ __device__
    T2 createMathObj(const double* comp);

    template  
    __host__ __device__
    T3 createMathObj(const double* comp);

    template<>
    __host__ __device__
    double createMathObj(const double* comp) {
        return *comp;
    }

    // --------------------------- Get number of components -------------------------------

    template<class Obj>
    __host__ __device__
    uint32_t getNumOfComp(const Obj& obj) {
        return obj.comp.size();
    }

    template  
    __host__ __device__
        uint32_t getNumOfComp(const V2& obj);

    template  
    __host__ __device__
        uint32_t getNumOfComp(const V3& obj);

    template  
    __host__ __device__
        uint32_t getNumOfComp(const T2& obj);

    template  
    __host__ __device__
        uint32_t getNumOfComp(const T2& obj);

    template<>
    __host__ __device__
    uint32_t getNumOfComp(const double& obj) {
        return 1;
    }

}