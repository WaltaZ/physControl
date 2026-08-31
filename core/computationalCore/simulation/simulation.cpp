#include <simulation/simulation.h>

#include <geometry/geometryUtils.h>

HeatTransferSimulationD3::HeatTransferSimulationD3(
	HeatTransferProblemD3& problem,
	Mesh<MeshDim::D3>& mesh) : _problem(problem), _mesh(mesh) {}

HeatTransferProblemD3& HeatTransferSimulationD3::getProblem()
{
	return _problem;
}

Mesh<MeshDim::D3>& HeatTransferSimulationD3::getMesh()
{
	return _mesh;
}

void HeatTransferSimulationD3::nextStep()
{
	// Update all the fields as a next step
	auto gradient = SimpleGradient();

	gradient.compute(_problem.fields.pressure, _problem.fields.gradPressure, _mesh);

	/*for (int i = 0; i < _problem.fields.gradPressure.values.length; i++) {
		geometryPrint::printV(_problem.fields.gradPressure.values[i]);
	}*/

	/*double* values = _problem.fields.pressure.values.getData();
	Vector<GeometryDim::D3>* destVector = _problem.fields.gradPressure.values.getData();
	CudaMesh<MeshDim::D3>* mesh = _mesh.getElements();
 
	for (int id = 0; id < mesh->cells.length; id++) {
		if (id < mesh->cells.length) {
		Vector<GeometryDim::D3> grad = Vector<GeometryDim::D3>({ 0, 0, 0 });
		const auto& cell = mesh->cells[id];

		for (int i = 0; i < cell.cellFaceIDs.length; i++) {
			const auto& face = mesh->faces[cell.cellFaceIDs[i]];

			if (!face.isBoundary) {

				double g_C = face.getWeightFactor(id);
				double g_F = 1 - g_C;

				double faceValue = (g_C * values[id]) + (g_F * values[face.getNeighbourCellID(id)]);
				grad = grad + (face.getArea(id).vector * faceValue);

				printf(
					"cell=%u face=%u owner=%u neighbour=%u "
					"area=(%.10f %.10f %.10f) "
					"valueC=%.10f valueN=%.10f\n",
					id,
					cell.cellFaceIDs[i],
					face.ownerCellID,
					face.getNeighbourCellID(id),
					face.getArea(id).vector.comp[0],
					face.getArea(id).vector.comp[1],
					face.getArea(id).vector.comp[2],
					values[id],
					values[face.getNeighbourCellID(id)]
				);
			}
		}
		destVector[id] = grad / cell.volume;
	}
	}*/

}
