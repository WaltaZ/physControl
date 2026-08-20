find_package(VTK REQUIRED)
vtk_module_autoinit( TARGETS ${PROJECT_NAME} MODULES ${VTK_LIBRARIES} )

find_package(CUDAToolkit REQUIRED)
include_directories(${CUDAToolkit_INCLUDE_DIRS})
link_directories(${CUDAToolkit_LIBRARY_DIR})