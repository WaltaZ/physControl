set(utilityHppFiles 
	"include/utility/utility.h"
	"include/utility/mathUtils.h"
	"include/utility/cudaUtils.h"
	"include/utility/cudaUtilsWithKernels.h"
	"include/utility/debugUtils.h"
	"include/utility/staticString.h"
	"include/utility/logger.h"
)

set(utilityCppFiles
	"mathUtils.cu"
	"cudaUtils.cu"
	"utility.cpp"
	"debugUtils.cpp"
	"logger.cpp"
)

add_library(utility STATIC
	${utilityHppFiles}
	${utilityCppFiles}
 )

target_include_directories(utility PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(utility PRIVATE 
geometry
mesh 
simulation
CUDA::cudart 
)