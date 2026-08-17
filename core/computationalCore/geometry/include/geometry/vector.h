#pragma once

template <GeometryDim dim>
class Vector : public Tensor<Vector<dim>, dim, 1> {
public:

	using ThisTensor = Tensor<Vector<dim>, dim, 1>;
	using ThisTensor::comp;
	using ThisTensor::_numOfComp;

	Vector() : ThisTensor(){};
	Vector(std::array<double, _numOfComp> comp) : ThisTensor(comp) {};
	Vector(Point<dim> point) : ThisTensor(point.pos) {};
	Vector(const Point<dim>& pointA, const Point<dim>& pointB) {
		comp = std::array<double, _numOfComp>{};
		for (int i = 0; i < _numOfComp; i++) {
			comp[i] = pointB.pos[i] - pointA.pos[i];
		};
	};

	double getMagnitude() {
		double mag = 0;
		for (int i = 0; i < _numOfComp; i++) {
			mag = mag + pow(comp[i], 2);
		}
		return sqrt(mag);
	}

	Vector<dim> getNormal() {
		return Vector<dim>(comp)/getMagnitude();
	}
};
