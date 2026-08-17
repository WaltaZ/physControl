add_library(geometry 
	"shapes/basic/point.h" 
	"vector.h"
	"tensor.h"
	"matrixTensor.h"
	"geometryUtils.h" 
	"shapes/basic/surface.h"
	"shapes/basic/line.h"
	"geometryDependencies.h"
	"tests.h"
	"geometryEnums.h"
	"shapes/basic/volume.h"
	"shapes/shapes.h"
	"geometry.h"
	"geometry.cpp"
)

target_include_directories(geometry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(geometry PRIVATE utility)