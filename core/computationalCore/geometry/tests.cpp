#include "include/geometry/tests.h"

#include "include/geometry/shapes/basic/point.h"
#include "include/geometry/shapes/basic/surface.h"
#include "include/geometry/shapes/basic/volume.h"
#include "include/geometry/shapes/basic/line.h"
#include "include/geometry/geometryUtils.h"

#include <iostream>

namespace geometryTests {
	void runSurfaceTest() {

		using P = Point<GeometryDim::D3>;
		using S = Surface<GeometryDim::D3>;

		std::vector<P*> points = {
			new P({1.0, 6.4, 0}),
			new P({2.4, 4.0, 0}),
			new P({2.0, 0.2, 0}),
			new P({0.4, 0, 0}),
			new P({0, 4.0, 0})
		};

		S surface(points);

		std::cout << "Area Vector: ";
		geometryPrint::printV(surface.getAreaVector());
		// Should be 0, 0, +/- 10.68

		std::cout << "Centroid: ";
		geometryPrint::printP(surface.getCentroid());
		// Should be 1.75, 2.88, 0

		std::cout << "Area: " << surface.getAreaVector().getMagnitude() << std::endl;
		// Should be 10.68
	}

	void runVolumeTest() {
		using P = Point<GeometryDim::D3>;
		using S = Surface<GeometryDim::D3>;
		using V = Volume;

		std::vector<P*> p = {
			new P({0, 0, 0}),
			new P({2, 0, 0}),
			new P({2, 2, 0}),
			new P({0, 2, 0}),
			new P({0, 0, 1}),
			new P({1, 0, 1}),
			new P({1, 1, 1}),
			new P({0, 1, 1}),
		};

		std::vector<S*> s = {
			new S({p[0], p[1], p[2], p[3]}),
			new S({p[4], p[5], p[6], p[7]}),
			new S({p[0], p[1], p[5], p[4]}),
			new S({p[1], p[2], p[6], p[5]}),
			new S({p[2], p[3], p[7], p[6]}),
			new S({p[3], p[0], p[4], p[7]}),
		};

		V volumeObj(p, s);

		P centroid = volumeObj.getCentroid();
		double volume = volumeObj.getVolume();

		std::cout << "Volume: " << volume << std::endl << "Centroid: ";
		// Should be 2.3333

		geometryPrint::printP(centroid);
		// Should be 0.8036, 0.8036, 0.3929
	}

	void runLineTest() {
		constexpr GeometryDim dim = GeometryDim::D2;

		using P = Point<dim>;
		using V = Vector<dim>;
		using L = Line<dim>;

		P p1({ 1, 1 });
		P p2({ 2, 3 });

		L l = L(&p1, &p2);

		double length = l.getLength();
		V areaVector = l.getAreaVector();
		P centroid = l.getCentroid();

		std::cout << "Line Length: " << length << std::endl;
		// Should be 2.23607

		std::cout << "Line area vector: ";
		geometryPrint::printV(areaVector);
		// Should be -2, 1

		std::cout << "Line centroid: ";
		geometryPrint::printP(centroid);
		// Should be 1.5, 2
	}

	void runColinearityTest() {
		using P2 = Point<GeometryDim::D2>;
		using P3 = Point<GeometryDim::D3>;

		P3 p1({ 0, 0, 0 });
		P3 p2({ 1, 1, 1 });
		P3 p3({ 5, 5, 5 });
		P3 p4({ 2, 1, 3 });

		std::cout << geometryOperations::arePointsColinear<GeometryDim::D3>({ &p1, &p2, &p3 }) << std::endl;
		// Should be true

		std::cout << geometryOperations::arePointsColinear<GeometryDim::D3>({ &p1, &p2, &p4 }) << std::endl;
		// Should be false

		std::vector<P3*> coplanar({
			new P3({0, 0, 0}),      // z = 0
			new P3({1, 0, 1}),      // z = 1
			new P3({0, 1, 2}),      // z = 2
			new P3({2, 1, 4}),      // z = 4
			new P3({-1, 2, 3}),     // z = 3
			new P3({3, -2, -1})     // z = -1
			});

		std::cout << geometryOperations::arePointsCoplanar(coplanar) << std::endl;
		// Should be true

		std::vector<P3*> nonCoplanar({
			new P3({0, 0, 0}),      // z = 0
			new P3({1, 0, 1}),      // z = 1
			new P3({0, 1, 2}),      // z = 2
			new P3({2, 1, 4}),      // z = 4
			new P3({-1, 2, 3}),     // z = 3
			new P3({3, -2, 0})      // should be z = -1, but is z = 0
			});

		std::cout << geometryOperations::arePointsCoplanar(nonCoplanar) << std::endl;
		// Should be false
	}
}