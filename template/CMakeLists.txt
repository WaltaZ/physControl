add_library(template "template.h" "template.cpp")

target_include_directories(template PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(template PRIVATE )