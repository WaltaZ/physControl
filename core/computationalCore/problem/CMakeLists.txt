set(problemHppFiles
	"include/problem/problem.h"
	"include/problem/problemGeometry.h"
	"include/problem/field.h"
	"include/problem/tests.h"
	"include/problem/boundaryInitialCondition.h"
)

set(problemCppFiles
	"field.cpp"
	"tests.cpp"
	"problem.cpp"
	"problemGeometry.cpp"
)

add_library(problem ${problemHppFiles} ${problemCppFiles})

target_include_directories(problem PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(problem PRIVATE geometry mesh)