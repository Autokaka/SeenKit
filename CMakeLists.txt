cmake_minimum_required(VERSION 3.8.0)
project(SpiritKit VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/build_config.cmake)

message(STATUS ${SEEN_VERSION})

if (${SEEN_BUILD_DARWIN})
  file(GLOB_RECURSE SOURCE
    "src/*.cc"
    "src/*.mm"
    "src/*.h"
    "include/*.h"
  )
  add_library(SeenKit STATIC ${SOURCE})
  target_include_directories(SeenKit PUBLIC public/darwin)
  target_include_directories(SeenKit PRIVATE src)
endif ()
