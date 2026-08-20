# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 4.3)

set(CMAKE_FASTBUILD_CACHE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cache")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(VCPKG_APPLOCAL_DEPS ON)

# CUDA
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --expt-relaxed-constexpr")

# Path to vcpkg
set(VCPKG_ROOT C:/dev/vcpkg)

# Include vcpkg libraries
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

set(PROJECT_NAME "physControl")
project (${PROJECT_NAME})

#physControl
add_subdirectory(core)
add_subdirectory(utility)
#add_subdirectory(app)

# Executable
add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE
	geometry
	mesh
	problem
	#utility
	#field
	vtkDisplay
)

include("Dependencies.cmake")