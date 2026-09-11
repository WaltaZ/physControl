# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 4.3)

#set(CMAKE_FASTBUILD_CACHE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cache")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(VCPKG_APPLOCAL_DEPS ON)

# CUDA
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --expt-relaxed-constexpr")
set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)

set(CMAKE_CUDA_USE_RESPONSE_FILE_FOR_OBJECTS 1)
set(CMAKE_CUDA_USE_RESPONSE_FILE_FOR_LINK_LIBRARIES 1)

# Path to vcpkg
set(VCPKG_ROOT C:/dev/vcpkg)

# Include vcpkg libraries
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

set(PROJECT_NAME "physControl")
project (${PROJECT_NAME} LANGUAGES CXX CUDA)

# Executable
add_executable(${PROJECT_NAME} "main.cu")

find_Package(CUDAToolkit)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	CUDA_SEPARABLE_COMPILATION ON
)

#physControl
add_subdirectory(core)
add_subdirectory(utility)

target_link_libraries(${PROJECT_NAME} PRIVATE
	geometry
	mesh
	problem
	simulation
	visuals
	utility
)