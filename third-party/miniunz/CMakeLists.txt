project(miniunz)

file(GLOB_RECURSE SOURCES
  "${PROJECT_SOURCE_DIR}/*.h"
  "${PROJECT_SOURCE_DIR}/*.c"
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES})
add_library(miniunz STATIC ${SOURCES})

target_include_directories(miniunz PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(miniunz PRIVATE ${PROJECT_SOURCE_DIR}/include/miniunz)
target_link_libraries(miniunz PRIVATE z)
