project(dispatch)

add_library(dispatch INTERFACE)
target_include_directories(javascriptcore INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(javascriptcore INTERFACE
  "${PROJECT_SOURCE_DIR}/lib/android/${ANDROID_ABI}/libdispatch.a"
)
