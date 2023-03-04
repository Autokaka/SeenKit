cmake_minimum_required(VERSION 3.8.0)
project(seen VERSION 1.0.0 LANGUAGES CXX OBJCXX Swift)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_INLINES_HIDDEN YES)
set(CMAKE_Swift_LANGUAGE_VERSION 5)
set(CPM_SOURCE_CACHE ${PROJECT_SOURCE_DIR}/.cache/cpm)

include(cmake/build_config.cmake)
include(cmake/cpm.cmake)

message(STATUS ${SEEN_VERSION})

add_subdirectory(third-party/glm)
add_subdirectory(third-party/miniunz)
add_subdirectory(third-party/wamr)

if(${SEEN_BUILD_DARWIN})
  file(GLOB_RECURSE PUB_HEADERS
    "${PROJECT_SOURCE_DIR}/public/darwin/*.h"
    )
  file(GLOB ARC_SOURCE
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cc"
    "${PROJECT_SOURCE_DIR}/src/*.mm"
    )
  file(GLOB_RECURSE MRC_SOURCE
    "${PROJECT_SOURCE_DIR}/src/seen/*.h"
    "${PROJECT_SOURCE_DIR}/src/seen/*.cc"
    "${PROJECT_SOURCE_DIR}/src/seen/*.mm"
    )
  file(GLOB_RECURSE SOURCE
    ${PUB_HEADERS}
    ${ARC_SOURCE}
    ${MRC_SOURCE}
    )
  set_source_files_properties(${ARC_SOURCE} PROPERTIES COMPILE_FLAGS -fobjc-arc)
  set_source_files_properties(${MRC_SOURCE} PROPERTIES COMPILE_FLAGS -fno-objc-arc)
  source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE})
  add_library(seen STATIC ${SOURCE})
  target_include_directories(seen PUBLIC public/darwin)
  target_include_directories(seen PRIVATE src)
  target_compile_definitions(seen PRIVATE ${SEEN_COMPILE_FLAGS})
  target_link_libraries(seen PRIVATE glm miniunz wamr)
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
  set_source_files_properties(${EXAMPLE_SOURCE} PROPERTIES COMPILE_FLAGS -fobjc-arc)
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
