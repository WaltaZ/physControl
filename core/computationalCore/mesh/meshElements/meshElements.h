#pragma once

class MeshElement {
public:
	virtual ~MeshElement() = default;
};

#include "supportStructs.h"
#include "node.h"
#include "face.h"
#include "cell.h"

// Testing
#include "tests.h"