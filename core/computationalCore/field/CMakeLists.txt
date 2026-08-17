add_library(field
	"field.h"
	"field.cpp"
 "fieldDependencies.h" "tests.h")

target_include_directories(field PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(field PRIVATE geometry mesh)