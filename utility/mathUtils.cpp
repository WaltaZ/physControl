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
}