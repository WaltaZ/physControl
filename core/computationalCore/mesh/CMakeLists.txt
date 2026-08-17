add_library(mesh
	"mesh.cpp"
	"meshers/mesher.h"
	"meshers/cartesianMesher/cartesianMesher.h"
	"meshEnums.h" 
	"mesh.h"
	"meshElements/meshElements.h"
	"meshElements/node.h"
	"meshElements/face.h"
	"meshElements/cell.h"
	"meshDependencies.h" "meshElements/tests.h")

target_include_directories(mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mesh geometry problem)