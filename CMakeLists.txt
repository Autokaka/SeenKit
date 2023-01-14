cmake_minimum_required(VERSION 3.8.0)
project(SeenKit VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CPM_SOURCE_CACHE .cache/cpm)

include(cmake/build_config.cmake)
include(cmake/cpm.cmake)

CPMAddPackage("https://github.com/bytecodealliance/wasm-micro-runtime.git#WAMR-1.1.2")

message(STATUS ${SEEN_VERSION})

if(${SEEN_BUILD_DARWIN})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-aligned-allocation")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "-std=c++20")
  if(IOS)
    set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "11.0")
    set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO)
  else()
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13")
  endif()
  file(GLOB_RECURSE SOURCE
    "public/darwin/*.h"
    "src/*.cc"
    "src/*.mm"
    "src/*.h"
  )
  add_library(SeenKit STATIC ${SOURCE})
  target_include_directories(SeenKit PUBLIC public/darwin)
  target_include_directories(SeenKit PRIVATE src)
  target_link_libraries(SeenKit PRIVATE iwasm_static)
endif()
