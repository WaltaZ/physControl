set(fileStreamHppFiles
	"include/fileStream/path.h"
	"include/fileStream/binaryFiles/binStream.h"
	"include/fileStream/binaryFiles/binWriter.h"
	"include/fileStream/binaryFiles/binReader.h"
)

set(fileStreamCppFiles
	"path.cpp"
)

add_library(fileStream
	${fileStreamHppFiles}
	${fileStreamCppFiles}
)

target_include_directories(fileStream PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(fileStream 
	utility
)