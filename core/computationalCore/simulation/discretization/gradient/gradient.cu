#include <simulation/discretization/gradient/gradient.h>

namespace gradUtils {

	template<typename Obj, typename GradObj>
	__device__
		V interpolateOnFace(
			uint32_t faceID,
			CudaField<Obj, C>* field,
			CudaField <GradObj, C>* gradField,
			CudaMesh<MeshDim::D3>* mesh)
	{
		const auto& face = mesh->faces[faceID];

		assert(!face.isBoundary);

		uint32_t C_ID = face.ownerCellID;
		uint32_t F_ID = face.neighbourCellID;

		const auto& C = mesh->cells[C_ID];
		const auto& F = mesh->cells[F_ID];

		const Obj& phi_C = field->values[C_ID];
		const Obj& phi_F = field->values[F_ID];

		const GradObj& grad_C = gradField->values[C_ID];
		const GradObj& grad_F = gradField->values[F_ID];

		double g_C = face.ownerFaceWeightFactor;
		double g_F = 1 - g_C;

		const double& d_CF = face.ownerToNeighbourCell.magnitude;
		const V& e_CF = face.ownerToNeighbourCell.normal;

		GradObj grad_bar = grad_C * g_C + grad_F * g_F;

		return
			grad_bar +
			(e_CF * (phi_F - phi_C) / d_CF) -
			(e_CF * geomOp::dotProduct(grad_bar, e_CF));
	}
}