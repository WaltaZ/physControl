#include <simulation/simulationCore/SIMPLE/SIMPLEKernel.h>

namespace SIMPLEKernel {

	using V = Vector<GeometryDim::D3>;
	using C = Cell<MeshDim::D3>;
	using F = Face<MeshDim::D3>;
	using M = MatrixTensor<GeometryDim::D3>;

	__global__
		void assembleInnerVelocity(
			const Mesh<MeshDim::D3>* mesh,
			HeatTransferFieldsD3* fields,
			LinearSolverMatrix<V>* matrix,
			HeatTransferSimulationMethods* methods
		)
	{

		int C_id = blockDim.x * blockIdx.x + threadIdx.x;

		if (C_id > mesh->cells.length) { return; }

		methods->diffusion->assembleInner(
			mesh,
			fields->velocity,
			matrix,
			simConfig::KINEMATIC_VISCOSITY
		);

		methods->convection->assembleInner(
			mesh,
			fields->velocity,
			fields->gradVelocity,
			fields->massFlowRate,
			matrix
		);

		methods->unsteady->assemble(
			mesh,
			fields->velocity,
			matrix,
			simConfig::DT
		);

		methods->sourceGravity->assembleInner(
			mesh,
			matrix,
			V({ 0, 0, -simConfig::G_CONSTANT })
		);

		/*methods->sourceBoussinesq->assembleInner(
			mesh,
			matrix,
			V({ 0, 0, -simConfig::G_CONSTANT }),
			fields->temperature,
			simConfig::THERM_EXPANSION
		);*/

		relaxation::patankar(matrix, fields->velocity, simConfig::V_RELAXATION_FACTOR);
		matrix->B[C_id] -= mesh->cells[C_id].volume * fields->gradPressure->values[C_id];

	};

	__global__
		void assembleBoundariesVelocity(
			const Mesh<MeshDim::D3>* mesh,
			HeatTransferFieldsD3* fields,
			LinearSolverMatrix<V>* matrix
		)
	{

		// Only Wall boundary condition and orthogonal grid for now.

		int bp_faceId = blockDim.x * blockIdx.x + threadIdx.x;

		auto& bp = fields->velocity->boundaryPatches[0];

		if (bp_faceId >= bp.faceIDs.length) { return; }

		const auto& f = mesh->faces[bp.faceIDs[bp_faceId]];

		if (!f.isBoundary) { return; }

		const auto& C_id = f.ownerCellID;
		const auto& C = mesh->cells[C_id];

		const auto V_C = fields->velocity->values[C_id];
		const auto& p_C = fields->pressure->values[C_id];
		const auto& gradP_C = fields->gradPressure->values[C_id];

		const VectorData<GeometryDim::D3> d_CF = f.ownerData.centroidToFace;

		auto& A_C = matrix->A_C[C_id];
		auto& B = matrix->B[C_id];

		const auto f_area = f.area;
		const double mu_S_over_d = 
			simConfig::KINEMATIC_VISCOSITY * f_area.magnitude 
			/ geomOp::dotProduct(d_CF.vector, f_area.normal);

		A_C += mu_S_over_d * (Vector<GeometryDim::D3>(1) - geomOp::hadProduct(f_area.normal, f_area.normal));

		Vector<GeometryDim::D3> B_contribution{};

		B_contribution[0] = (V_C[1] * f_area.normal[1] * f_area.normal[0]) + (V_C[2] * f_area.normal[2] * f_area.normal[0]);
		B_contribution[1] = (V_C[0] * f_area.normal[0] * f_area.normal[1]) + (V_C[2] * f_area.normal[2] * f_area.normal[1]);
		B_contribution[2] = (V_C[0] * f_area.normal[0] * f_area.normal[2]) + (V_C[1] * f_area.normal[1] * f_area.normal[2]);

		B_contribution *= mu_S_over_d;

		B += B_contribution;
	};

	__global__
		void updateMassFlow(
			const Mesh<MeshDim::D3>* mesh,
			HeatTransferFieldsD3* fields,
			LinearSolverMatrix<V>* matrix
		)
	{
		int f_id = blockDim.x * blockIdx.x + threadIdx.x;

		if (f_id >= mesh->faces.length) { return; }

		const auto& f = mesh->faces[f_id];

		if (f.isBoundary) {
			fields->massFlowRate->values[f_id] = 0;
			return; 
		}

		const VectorData<GeometryDim::D3> fArea = f.area;
		const VectorData<GeometryDim::D3> d_CF = f.ownerToNeighbourCell;

		const uint32_t C_id = f.ownerCellID;
		const uint32_t F_id = f.neighbourCellID;

		double g_C = f.ownerFaceWeightFactor;
		double g_F = 1 - g_C;

		const Vector<GeometryDim::D3> gradP_f_bar =
			(fields->gradPressure->values[C_id] * g_C)
			+ (fields->gradPressure->values[F_id] * g_F); // Use values from previous SIMPLE iteration?

		const Vector<GeometryDim::D3> V_f_bar =
			(fields->velocity->values[C_id] * g_C)
			+ (fields->velocity->values[F_id] * g_F);

		const Vector<GeometryDim::D3> gradP_f =
			(
				(
					(fields->pressure->values[F_id] - fields->pressure->values[C_id])
					/ d_CF.magnitude
				)
				- geomOp::dotProduct(gradP_f_bar, d_CF.normal)
			) * d_CF.normal;

		// Rhie-Chow interpolation
		const Vector<GeometryDim::D3> Vol_C{ mesh->cells[C_id].volume };
		const Vector<GeometryDim::D3> Vol_F{ mesh->cells[F_id].volume };

		const Vector<GeometryDim::D3> D_C = geomOp::hadDivision(Vol_C, matrix->A_C[C_id]);
		const Vector<GeometryDim::D3> D_F = geomOp::hadDivision(Vol_F, matrix->A_C[F_id]);
		const Vector<GeometryDim::D3> D_f_bar = (D_C * g_C) + (D_F * g_F);

		const Vector<GeometryDim::D3> V_f = V_f_bar - geomOp::hadProduct(D_f_bar, gradP_f);

		fields->massFlowRate->values[f_id] = geomOp::dotProduct(V_f, fArea.vector);

		// Seems right
	};

	__global__
		void assemblePressure(
			const Mesh<MeshDim::D3>* mesh,
			HeatTransferFieldsD3* fields,
			LinearSolverMatrix<double>* p_matrix,
			LinearSolverMatrix<Vector<GeometryDim::D3>>* V_matrix
		)
	{
		int C_id = blockDim.x * blockIdx.x + threadIdx.x;

		if (C_id >= mesh->cells.length) { return; }

		const auto& C = mesh->cells[C_id];

		double A_C_contribution = 0;
		double B_contribution = 0;

		auto& A_C = p_matrix->A_C[C_id];
		auto& A_F = p_matrix->A_F[C_id];
		auto& B = p_matrix->B[C_id];

		const Vector<GeometryDim::D3> Vol_C{ C.volume };
		const Vector<GeometryDim::D3> D_C = geomOp::hadDivision(Vol_C, V_matrix->A_C[C_id]);

		for (size_t i = 0; i < A_F.length; i++)
		{
			uint32_t F_id = C.cellNeighbourCells[i];

			uint32_t f_id = mesh->getCommonFaceId(C, i);
			const auto& f = mesh->faces[f_id];

			const VectorData<GeometryDim::D3> d_CF = f.getCellToNeighbourVector(C_id);

			const double g_C = f.getWeightFactor(C_id);
			const double g_F = 1 - g_C;

			const Vector<GeometryDim::D3> Vol_F{ mesh->cells[F_id].volume };
			const Vector<GeometryDim::D3> D_F = geomOp::hadDivision(Vol_F, V_matrix->A_C[F_id]);
			const Vector<GeometryDim::D3> D_f_bar = (D_C * g_C) + (D_F * g_F);

			const Vector<GeometryDim::D3> S_f_prime = geomOp::hadProduct(D_f_bar, f.area.vector);

			double A_F_contribution = -S_f_prime.getMagnitude() / d_CF.magnitude;

			A_C_contribution -= A_F_contribution;

			A_F[i] = A_F_contribution;

			double m_f = fields->massFlowRate->values[f_id];

			if (C_id == f.neighbourCellID) { m_f = -m_f; }
			B_contribution -= m_f;
		}
		B = B_contribution;
		A_C = A_C_contribution;
	};

	__global__
		void correctPressureAndVelocity(
			const Mesh<MeshDim::D3>* mesh,
			HeatTransferFieldsD3* fields,
			Field<double, C>* pressureCorr,
			Field<V, C>* gradPressureCorr,
			LinearSolverMatrix<V>* V_matrix
		)
	{
		int C_id = blockDim.x * blockIdx.x + threadIdx.x;

		if (C_id >= mesh->cells.length) { return; }

		fields->pressure->values[C_id] += 
			simConfig::P_RELAXATION_FACTOR * pressureCorr->values[C_id];

		const V Vol{ mesh->cells[C_id].volume };

		const V D_C = geomOp::hadDivision(Vol, V_matrix->A_C[C_id]);

		fields->velocity->values[C_id] -= geomOp::hadProduct(D_C, gradPressureCorr->values[C_id]);
	};

	__global__
		void correctMassFlow(
			const Mesh<MeshDim::D3>* mesh,
			HeatTransferFieldsD3* fields,
			Field<double, C>* PCorr,
			Field<V, C>* gradPCorr,
			LinearSolverMatrix<V>* V_matrix
		)
	{
		int f_id = blockDim.x * blockIdx.x + threadIdx.x;

		if (f_id >= mesh->faces.length) { return; }

		const auto& f = mesh->faces[f_id];

		if (f.isBoundary) { return; }

		const VectorData<GeometryDim::D3> fOwn2Neighb = f.ownerToNeighbourCell;

		const uint32_t C_id = f.ownerCellID;
		const uint32_t F_id = f.neighbourCellID;

		const double g_C = f.ownerFaceWeightFactor;
		const double g_F = 1 - g_C;

		// Rhie-Chow interpolation
		const Vector<GeometryDim::D3> Vol_C{ mesh->cells[C_id].volume };
		const Vector<GeometryDim::D3> Vol_F{ mesh->cells[F_id].volume };

		const Vector<GeometryDim::D3> D_C = geomOp::hadDivision(Vol_C, V_matrix->A_C[C_id]);
		const Vector<GeometryDim::D3> D_F = geomOp::hadDivision(Vol_F, V_matrix->A_C[F_id]);
		const Vector<GeometryDim::D3> D_f_bar = (D_C * g_C) + (D_F * g_F);

		const Vector<GeometryDim::D3> gradPCorr_f_bar =
			(gradPCorr->values[C_id] * g_C)
			+ (gradPCorr->values[F_id] * g_F);

		const Vector<GeometryDim::D3> gradPCorr_f =
			(
				(
					(PCorr->values[F_id] - PCorr->values[C_id])
					/ fOwn2Neighb.magnitude
				)
				- geomOp::dotProduct(gradPCorr_f_bar, fOwn2Neighb.normal)
			) * fOwn2Neighb.normal + gradPCorr_f_bar;

		fields->massFlowRate->values[f_id] -= geomOp::dotProduct(geomOp::hadProduct(D_f_bar, gradPCorr_f), f.area.vector);
	};

	template<class Obj>
	__global__
		void resetMatrix(
			LinearSolverMatrix<Obj>* matrix
		) 
	{
		int C_id = blockDim.x * blockIdx.x + threadIdx.x;

		if (C_id >= matrix->A_C.length) { return; }

		Obj obj{};

		matrix->A_C[C_id] = obj;
		matrix->B[C_id] = obj;

		for (size_t i = 0; i < matrix->A_F[C_id].length; i++)
		{
			matrix->A_F[C_id][i] = obj;
		}
	};

	template
	__global__
		void resetMatrix(
			LinearSolverMatrix<double>* matrix
		);

	template
	__global__
		void resetMatrix(
			LinearSolverMatrix<Vector<GeometryDim::D3>>* matrix
		);

}