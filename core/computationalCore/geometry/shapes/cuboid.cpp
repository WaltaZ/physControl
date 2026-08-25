#include "../include/geometry/shapes/cuboid.h"

Cuboid::Cuboid(
		const double& a,
		const double& b,
		const double& c,
		const Point<GeometryDim::D3>& startingPoint)
	:
		a(a),
		b(b),
		c(c)
{
	points = std::vector<P*>({
		new P({0, 0, 0}), // 0
		new P({a, 0, 0}), // 1
		new P({a, b, 0}), // 2
		new P({0, b, 0}), // 3
		new P({0, 0, c}), // 4
		new P({a, 0, c}), // 5
		new P({a, b, c}), // 6
		new P({0, b, c}) // 7
		});

	for (auto& p : points) {
		*p = *p + startingPoint;
	}

	surfaces = std::vector<S*>({
		new S({points[1], points[2], points[6], points[5]}), // YZ Front
		new S({points[3], points[0], points[4], points[7]}), // YZ Back
		new S({points[2], points[3], points[7], points[6]}), // XZ Right
		new S({points[0], points[1], points[5], points[4]}), // XZ Left
		new S({points[4], points[5], points[6], points[7]}), // XY Top 
		new S({points[0], points[1], points[2], points[3]})  // XY Bottom
		});
}

const Cuboid::FaceType Cuboid::faceOrder[6] = {
		FaceType::Front, // 0
		FaceType::Back, // 1
		FaceType::Right, // 2
		FaceType::Left, // 3
		FaceType::Top, // 4
		FaceType::Bottom // 5
};