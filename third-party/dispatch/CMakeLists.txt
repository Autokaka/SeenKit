project(dispatch)

add_library(dispatch INTERFACE)
target_include_directories(dispatch INTERFACE ${PROJECT_SOURCE_DIR}/include)

if (ANDROID)
  target_link_libraries(dispatch INTERFACE
    "${PROJECT_SOURCE_DIR}/lib/android/${ANDROID_ABI}/libdispatch.a"
  )
endif()
