#pragma once

template<typename Derived, GeometryDim dim, int rank>
class Tensor {
protected:
	static constexpr int _numOfComp = [] {
		int result = 1;
		for (int i = 0; i < rank; i++)
			result *= geometryDimSize(dim);
		return result;
		}();
public:
	std::array<double, _numOfComp> comp;

	Tensor()
	{
		comp = std::array<double, _numOfComp>{};
		std::fill(std::begin(comp), std::end(comp), 0);
	};

	Tensor(std::array<double, _numOfComp> comp) : comp(comp) {};

	// OPERATIONS ------------------------------------

	double& operator[](int index) {
		return comp[index];
	}

	const double& operator[](int index) const {
		return comp[index];
	}

	Derived operator+(const Derived& tensor) const
	{
		std::array<double, _numOfComp> finalComp{};
		for (int i = 0; i < _numOfComp; i++) {
			finalComp[i] = comp[i] + tensor.comp[i];
		}
		return Derived(finalComp);
	};

	Derived operator-(const Derived& tensor) const
	{
		std::array<double, _numOfComp> finalComp{};
		for (int i = 0; i < _numOfComp; i++) {
			finalComp[i] = comp[i] - tensor.comp[i];
		}
		return Derived(finalComp);
	};

	Derived operator*(const double& scalar) const
	{
		std::array<double, _numOfComp> finalComp{};
		for (int i = 0; i < _numOfComp; i++) {
			finalComp[i] = comp[i] * scalar;
		}
		return Derived(finalComp);
	}

	Derived operator/(const double& scalar) const
	{
		std::array<double, _numOfComp> finalComp{};
		for (int i = 0; i < _numOfComp; i++) {
			finalComp[i] = comp[i] / scalar;
		}
		return Derived(finalComp);
	}

	bool operator==(const Derived& tensor) const
	{
		bool isTheSame = true;
		for (int i = 0; i < _numOfComp; i++) {
			isTheSame = (comp[i] == tensor.comp[i]);
			if (!isTheSame) { break; }
		}
		return isTheSame;
	}
};