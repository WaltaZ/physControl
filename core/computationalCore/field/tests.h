namespace fieldTests {
	void runFieldBasicTest() {
		// Example usage of Field class
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3> > field(10); // Field of Vectors associated with 10 points in 3D
		// Set some values
		for (int i = 0; i < field.numberOfElements; i++) {
			field.data[i] = field.data[i] + (Vector<GeometryDim::D3>({1, 1, 1}) * i*1.5); // Assigning some values
		}
		// Print the values
		std::cout << "Vector Values: " << std::endl;
		for (int i = 0; i < field.numberOfElements; i++) {
			std::cout << "V" << i << ": ";
			geometryPrint::printV(field.data[i]);
		}
		std::cout << std::endl;
	}

	void runFieldInitializationTest() {
		// Example usage of Field class with initialization
		Field<Vector<GeometryDim::D3>, Cell<MeshDim::D3> > field(5, Vector<GeometryDim::D3>({ 21, 37, 69 })); // Field of Vectors associated with 5 points in 3D, initialized to some value
		// Print the values
		std::cout << "Initialized Vector Values: " << std::endl;
		for (int i = 0; i < field.numberOfElements; i++) {
			std::cout << "V" << i << ": ";
			geometryPrint::printV(field.data[i]);
		}
		std::cout << std::endl;
	}
}