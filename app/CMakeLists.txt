set(appHppFiles
	"include/app/app.h"
)

set(appCppFiles
	"app.cu"
	"_.cpp"
)

add_library(app 
	${appHppFiles}
	${appCppFiles}
)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(app 
	geometry
	mesh
	problem
	simulation
	visuals
	utility
)