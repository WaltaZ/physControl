find_package(VTK REQUIRED)
vtk_module_autoinit( TARGETS ${PROJECT_NAME} MODULES ${VTK_LIBRARIES} )