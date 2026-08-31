set(meshHppFiles
	"include/mesh/mesh.h"
	"include/mesh/meshEnums.h"
	"include/mesh/meshElements/mesh.h"
	"include/mesh/meshElements/tests.h"
	"include/mesh/meshElements/supportStructs.h"
	"include/mesh/meshElements/meshElements.h"
	"include/mesh/meshElements/face.h"
	"include/mesh/meshElements/cell.h"
	"include/mesh/meshElements/node.h"
	"include/mesh/meshers/meshers.h"
	"include/mesh/meshers/cartesianMesher/cartesianMesher.h"
	"include/mesh/meshers/mesherElements.h"
)

set(meshCppFiles
	"meshers/cartesianMesher/cartesianMesher.cpp"
	"meshElements/face.cu"
	"meshElements/mesh.cpp"
	"meshElements/node.cpp"
	"meshElements/tests.cpp"
	"meshElements/supportStructs.cpp"
	"meshers/mesherElements.cpp"
	"meshElements/cell.cpp"
)

add_library(mesh 
	${meshHppFiles}
	${meshCppFiles}
)

target_include_directories(mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(mesh geometry problem utility CUDA::cudart)