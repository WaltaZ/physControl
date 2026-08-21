#pragma once

#include <vector>

namespace mathUtils {
    std::vector<double> linearlyInterpolatePointsWithSpacing(const std::vector<double>& points, double spacing);
}
/*
class CubicSpline {
public:
    // x must be strictly increasing (e.g. 0,1,2,3,... for scalar values by index)
    CubicSpline(const std::vector<double>& x, const std::vector<double>& y) {
        assert(x.size() == y.size() && x.size() >= 2);
        x_ = x;
        y_ = y;
        buildCoefficients();
    }

    // Evaluate the spline at a single point (clamped to [x_.front(), x_.back()])
    double operator()(double xi) const {
        xi = std::clamp(xi, x_.front(), x_.back());
        size_t i = findSegment(xi);
        double dx = xi - x_[i];
        // s(x) = a + b*dx + c*dx^2 + d*dx^3
        return a_[i] + b_[i] * dx + c_[i] * dx * dx + d_[i] * dx * dx * dx;
    }

    // Sample `count` evenly-spaced points across the full range
    std::vector<double> sample(int count) const {
        std::vector<double> result;
        result.reserve(count);
        double x0 = x_.front();
        double x1 = x_.back();
        for (int i = 0; i < count; ++i) {
            double t = (count == 1) ? 0.0 : double(i) / double(count - 1);
            double xi = x0 + t * (x1 - x0);
            result.push_back((*this)(xi));
        }
        return result;
    }

private:
    std::vector<double> x_, y_;
    std::vector<double> a_, b_, c_, d_; // per-segment coefficients

    // Binary search for the segment index i such that x_[i] <= xi <= x_[i+1]
    size_t findSegment(double xi) const {
        size_t n = x_.size();
        size_t lo = 0, hi = n - 2; // segments are [0, n-2]
        while (lo < hi) {
            size_t mid = (lo + hi + 1) / 2;
            if (x_[mid] <= xi) lo = mid;
            else hi = mid - 1;
        }
        return lo;
    }

    // Solve the tridiagonal system for a natural cubic spline (second
    // derivative = 0 at both endpoints) via the standard Thomas algorithm.
    void buildCoefficients() {
        size_t n = x_.size();
        size_t segs = n - 1;

        std::vector<double> h(segs);
        for (size_t i = 0; i < segs; ++i) h[i] = x_[i + 1] - x_[i];

        // Right-hand side and tridiagonal coefficients for the second
        // derivatives (c array, size n)
        std::vector<double> alpha(n, 0.0);
        for (size_t i = 1; i < segs; ++i) {
            alpha[i] = 3.0 * ((y_[i + 1] - y_[i]) / h[i] -
                (y_[i] - y_[i - 1]) / h[i - 1]);
        }

        std::vector<double> l(n), mu(n), z(n);
        l[0] = 1.0; mu[0] = 0.0; z[0] = 0.0;
        for (size_t i = 1; i < segs; ++i) {
            l[i] = 2.0 * (x_[i + 1] - x_[i - 1]) - h[i - 1] * mu[i - 1];
            mu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
        }
        l[n - 1] = 1.0; z[n - 1] = 0.0;

        std::vector<double> c(n, 0.0), b(segs), d(segs), a(segs);
        for (size_t j = segs; j-- > 0;) {
            c[j] = z[j] - mu[j] * c[j + 1];
            b[j] = (y_[j + 1] - y_[j]) / h[j] - h[j] * (c[j + 1] + 2.0 * c[j]) / 3.0;
            d[j] = (c[j + 1] - c[j]) / (3.0 * h[j]);
            a[j] = y_[j];
        }

        a_ = a; b_ = b; d_ = d;
        c_.assign(c.begin(), c.end() - 1); // one coefficient per segment
    }
};*/