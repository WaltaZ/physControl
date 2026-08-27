find_package(VTK REQUIRED)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "CylinderExample: Unable to find the VTK build folder.")
endif()

set(libName "visuals")

set(libHppFiles
	"include/visuals/displayer/displayer.h"
	"include/visuals/displayer/keyPressInteractor.h"
	"include/visuals/displayer/scene.h"
	"include/visuals/adapters.h"
	"include/visuals/visuals.h"
)

set(libCppFiles
	"displayer/displayer.cpp"
	"displayer/keyPressInteractor.cpp"
	"displayer/scene.cpp"
	"adapters.cpp"
)

add_library(${libName} ${libHppFiles} ${libCppFiles})
target_include_directories(${libName} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${libName} PUBLIC
${VTK_LIBRARIES}
mesh
)

# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS ${libName}
  MODULES ${VTK_LIBRARIES}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${libName} PROPERTY CXX_STANDARD 20)
endif()