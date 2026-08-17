add_library(problem "problem.h" "problem.cpp")

target_include_directories(problem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(problem PRIVATE geometry)