find_package(VTK REQUIRED)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "CylinderExample: Unable to find the VTK build folder.")
endif()

# Add source to this project's executable.
set(vtkDisplayHppFiles
	"include/vtkDisplay/vtkDisplay.h"
	"include/vtkDisplay/meshAdapters.h"
)

set(vtkDisplayCppFiles
	"vtkDisplay.cpp"
	"meshAdapters.cpp"
)

add_library(vtkDisplay ${vtkDisplayHppFiles} ${vtkDisplayCppFiles})
target_include_directories(vtkDisplay PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(vtkDisplay PUBLIC
${VTK_LIBRARIES}
mesh
)

# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS vtkDisplay
  MODULES ${VTK_LIBRARIES}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET vtkDisplay PROPERTY CXX_STANDARD 20)
endif()