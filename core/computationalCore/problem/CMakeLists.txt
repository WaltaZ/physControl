set(problemHppFiles
	"include/problem/problem.h"
	"include/problem/problemGeometry.h"
	"include/problem/field/field.h"
	
	"include/problem/field/boundaryPatch.h"
	"include/problem/tests.h"
	"include/problem/boundaryCondition.h"
)

set(problemCppFiles
	"field/fieldKernel.cu"
	"field/boundaryPatch.cu"
	"field/field.cu"
	"tests.cpp"
	"problem.cpp"
	"problemGeometry.cpp"
	"boundaryCondition.cpp"
)

add_library(problem ${problemHppFiles} ${problemCppFiles})

target_include_directories(problem PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(problem PRIVATE geometry mesh)