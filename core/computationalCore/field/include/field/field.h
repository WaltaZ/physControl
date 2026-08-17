#pragma once
#include "fieldDependencies.h"

template<typename DataType, typename StoragePlace>
struct Field {
    static_assert(
        std::is_base_of_v<MeshElement, StoragePlace>,
        "Storage place must be an element of Mesh!"
        );
	const int numberOfElements;
	std::vector<DataType> data;

	Field(const int numberOfElements, DataType initialObject = DataType()) : numberOfElements(numberOfElements), data(numberOfElements) {
		for (int i = 0; i < numberOfElements; i++) {
			data[i] = initialObject;
		}
	}
};

#include "tests.h"