set(geometryHppFiles
	"include/geometry/geometry.h"
	"include/geometry/geometryEnums.h"
	"include/geometry/geometryUtils.h"
	"include/geometry/tensor.h"
	"include/geometry/matrixTensor.h"
	"include/geometry/vector.h"
	"include/geometry/tests.h"
	"include/geometry/shapes/shapes.h"
	"include/geometry/shapes/cuboid.h"
	"include/geometry/shapes/basic/point.h"
	"include/geometry/shapes/basic/line.h"
	"include/geometry/shapes/basic/surface.h"
	"include/geometry/shapes/basic/volume.h"
)

set(geometryCppFiles
	"geometryUtils.cpp"
	"matrixTensor.cpp"
	"tests.cpp"
	"vector.cpp"
	"shapes/cuboid.cpp"
	"shapes/basic/point.cpp"
	"shapes/basic/line.cpp"
	"shapes/basic/surface.cpp"
	"shapes/basic/volume.cpp"
)

add_library(geometry 
	${geometryHppFiles}
	${geometryCppFiles}
)

target_include_directories(geometry PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(geometry PRIVATE utility)