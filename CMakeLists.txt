cmake_minimum_required(VERSION 3.8.0)
project(SeenKit VERSION 1.0.0 LANGUAGES CXX OBJCXX Swift)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_Swift_LANGUAGE_VERSION 5)
set(CPM_SOURCE_CACHE ${PROJECT_SOURCE_DIR}/.cache/cpm)

include(cmake/build_config.cmake)
include(cmake/cpm.cmake)

CPMAddPackage("https://github.com/bytecodealliance/wasm-micro-runtime.git#WAMR-1.1.2")

message(STATUS ${SEEN_VERSION})

if(${SEEN_BUILD_DARWIN})
  if(${CMAKE_VERSION} VERSION_LESS "3.20.0")
    error("You need 3.20.0 or higher version of CMake to build SeenKit on iOS or macOS.")
  endif()

  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "-std=c++20")
  set(SEEN_BUNDLE_ID graphics.seen)
  if(${SEEN_BUILD_IOS})
    set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "11.0")
    set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO)
  else()
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")
  endif()

  file(GLOB_RECURSE PUBLIC_HEADERS
    "public/darwin/*.h"
  )
  file(GLOB_RECURSE SOURCE
    ${PUBLIC_HEADERS}
    "src/*.cc"
    "src/*.mm"
    "src/*.h"
  )
  source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE})
  add_library(SeenKit STATIC ${SOURCE})
  target_include_directories(SeenKit PUBLIC public/darwin)
  target_include_directories(SeenKit PRIVATE src)
  target_link_libraries(SeenKit PRIVATE iwasm_static)
  set_target_properties(SeenKit PROPERTIES
    OUTPUT_NAME SeenKit
    FRAMEWORK TRUE
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    MACOSX_FRAMEWORK_IDENTIFIER "${SEEN_BUNDLE_ID}"
    MACOSX_FRAMEWORK_BUNDLE_VERSION "${SEEN_VERSION}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${PROJECT_VERSION}"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${SEEN_BUNDLE_ID}"
    XCODE_ATTRIBUTE_CODE_SIGN_STYLE Automatic
  )

  # example
  add_subdirectory(example/darwin)
  file(GLOB_RECURSE EXAMPLE_SOURCE 
    "${example_SOURCE_DIR}/*.h"
    "${example_SOURCE_DIR}/*.swift"
  )
  source_group(TREE ${example_SOURCE_DIR} FILES ${EXAMPLE_SOURCE})
  add_executable(example MACOSX_BUNDLE ${EXAMPLE_SOURCE})
  target_link_libraries(example PRIVATE SeenKit)
  set_target_properties(example PROPERTIES
    XCODE_ATTRIBUTE_SWIFT_OBJC_BRIDGING_HEADER "${example_SOURCE_DIR}/BridgingHeaders.h"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${SEEN_BUNDLE_ID}.example"
    XCODE_ATTRIBUTE_ENABLE_PREVIEWS YES
    XCODE_ATTRIBUTE_SWIFT_OPTIMIZATION_LEVEL -Onone
    XCODE_ATTRIBUTE_CODE_SIGN_STYLE Automatic
  )
endif()
