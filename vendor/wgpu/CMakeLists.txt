project(wgpu)

add_library(wgpu INTERFACE)

target_include_directories(wgpu INTERFACE include)

if(${SEEN_BUILD_MACOS})
  target_link_libraries(wgpu INTERFACE 
    "${PROJECT_SOURCE_DIR}/lib/macos/${CMAKE_SYSTEM_PROCESSOR}/libwgpu_native.a"
  )
endif()
