add_library(utility STATIC
	"utility.h"
	"mathUtils.h"
	"mathUtils.cpp"
	"debug/debug.cpp"
	)

target_include_directories(utility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(utility PRIVATE geometry mesh)