#include <simulation/discretization/interpolation/interpolationTaylor.h>

namespace interpolation {

	template<class Obj, class GradObj>
	__device__
		Obj taylor(
			const Obj& phi,
			const GradObj& gradPhi,
			const Vector<GeometryDim::D3>& direction
		) 
	{
		return phi + geomOp::dotProduct(gradPhi, direction);
	};

	template
		__device__
		double taylor(
			const double& phi,
			const Vector<GeometryDim::D3>& gradPhi,
			const Vector<GeometryDim::D3>& direction
		);

	template
		__device__
		Vector<GeometryDim::D3> taylor(
			const Vector<GeometryDim::D3>& phi,
			const MatrixTensor<GeometryDim::D3>& gradPhi,
			const Vector<GeometryDim::D3>& direction
		);
}

	