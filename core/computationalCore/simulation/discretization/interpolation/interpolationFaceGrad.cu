#include <simulation/discretization/interpolation/interpolationFaceGrad.h>

namespace interpolation {

	__device__
		Vector<GeometryDim::D3> faceGradient(
			const Face<MeshDim::D3>& f,
			Mesh<MeshDim::D3>* mesh,
			Field<double, Cell<MeshDim::D3>> phi,
			Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3>> gradPhi
		) 
	{
		assert(!f.isBoundary);

		// TODO: Implement a memory-friendly version
	};
}