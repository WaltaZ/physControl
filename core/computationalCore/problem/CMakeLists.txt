set(problemHppFiles
	"include/problem/problem.h"
	"include/problem/problemGeometry.h"
)

#set(geometryCppFiles)

add_library(problem ${problemHppFiles} "test.cpp")

target_include_directories(problem PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(problem PRIVATE geometry)