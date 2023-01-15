cmake_minimum_required(VERSION 3.8.0)
project(seen VERSION 1.0.0 LANGUAGES CXX OBJCXX Swift)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_Swift_LANGUAGE_VERSION 5)
set(CPM_SOURCE_CACHE ${PROJECT_SOURCE_DIR}/.cache/cpm)

include(cmake/build_config.cmake)
include(cmake/cpm.cmake)

CPMAddPackage("https://github.com/bytecodealliance/wasm-micro-runtime.git#WAMR-1.1.2")
CPMAddPackage("https://github.com/Autokaka/zlib.git#master")

message(STATUS ${SEEN_VERSION})

if(${SEEN_BUILD_DARWIN})
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
  add_library(seen STATIC ${SOURCE})
  target_include_directories(seen PUBLIC public/darwin)
  target_include_directories(seen PRIVATE src)
  target_compile_definitions(seen PRIVATE ${SEEN_ENV_DEFS})
  target_link_libraries(seen PRIVATE iwasm_static miniunz)
  set_target_properties(seen PROPERTIES
    FRAMEWORK TRUE
    MACOSX_DEPLOYMENT_TARGET "11.0"
    MACOSX_FRAMEWORK_IDENTIFIER "${SEEN_BUNDLE_ID}"
    MACOSX_FRAMEWORK_BUNDLE_VERSION "${SEEN_VERSION}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${PROJECT_VERSION}"
    OUTPUT_NAME SeenKit
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "-std=c++20"
    XCODE_ATTRIBUTE_CODE_SIGN_STYLE Automatic
    XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "11.0"
    XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${SEEN_BUNDLE_ID}"
  )

  # example
  add_subdirectory(example/darwin)
  file(GLOB_RECURSE EXAMPLE_SOURCE 
    "${seen-example_SOURCE_DIR}/*.h"
    "${seen-example_SOURCE_DIR}/*.swift"
  )
  source_group(TREE ${seen-example_SOURCE_DIR} FILES ${EXAMPLE_SOURCE})
  add_executable(seen-example MACOSX_BUNDLE ${EXAMPLE_SOURCE})
  target_link_libraries(seen-example PRIVATE seen)
  set_target_properties(seen-example PROPERTIES
    XCODE_ATTRIBUTE_CODE_SIGN_STYLE Automatic
    XCODE_ATTRIBUTE_ENABLE_PREVIEWS YES
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${SEEN_BUNDLE_ID}.example"
    XCODE_ATTRIBUTE_SWIFT_OBJC_BRIDGING_HEADER "${seen-example_SOURCE_DIR}/BridgingHeaders.h"
    XCODE_ATTRIBUTE_SWIFT_OPTIMIZATION_LEVEL -Onone
  )
endif()
