set(utilityHppFiles 
	"include/utility/utility.h"
	"include/utility/mathUtils.h"
	"include/utility/cudaUtils.h"
)

set(utilityCppFiles
	"mathUtils.cpp"
)

add_library(utility STATIC
	${utilityHppFiles}
	${utilityCppFiles}
)

target_include_directories(utility PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(utility PRIVATE geometry mesh CUDA::cudart)