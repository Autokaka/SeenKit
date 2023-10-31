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

add_subdirectory(vendor/fmt)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/miniz)
add_subdirectory(vendor/wamr)

if(${SEEN_BUILD_DARWIN})
  add_subdirectory(lib/darwin)
  file(GLOB_RECURSE PUB_HEADER 
    "${darwin_SOURCE_DIR}/include/*.h"
  )
  file(GLOB_RECURSE ARC_SOURCE
    "${darwin_SOURCE_DIR}/*.h"
    "${darwin_SOURCE_DIR}/*.mm"
  )
  file(GLOB_RECURSE MRC_SOURCE
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cc"
    "${PROJECT_SOURCE_DIR}/src/*.mm"
  )
  file(GLOB_RECURSE SOURCE
    ${PUB_HEADER}
    ${ARC_SOURCE}
    ${MRC_SOURCE}
  )
  set_source_files_properties(${ARC_SOURCE} PROPERTIES COMPILE_FLAGS -fobjc-arc)
  set_source_files_properties(${MRC_SOURCE} PROPERTIES COMPILE_FLAGS -fno-objc-arc)
  source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE})
  add_library(seen STATIC ${SOURCE})
  target_include_directories(seen PUBLIC ${darwin_SOURCE_DIR}/include)
  target_include_directories(seen PRIVATE src)
  target_compile_definitions(seen PUBLIC ${SEEN_COMPILE_FLAGS})
  target_link_libraries(seen PRIVATE fmt glm miniz wamr)
  set_target_properties(seen PROPERTIES
    FRAMEWORK TRUE
    MACOSX_DEPLOYMENT_TARGET "11.0"
    MACOSX_FRAMEWORK_IDENTIFIER "${SEEN_BUNDLE_ID}"
    MACOSX_FRAMEWORK_BUNDLE_VERSION "${SEEN_VERSION}"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${PROJECT_VERSION}"
    OUTPUT_NAME SeenKit
    PUBLIC_HEADER "${PUB_HEADER}"
    XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "-std=c++20"
    XCODE_ATTRIBUTE_CODE_SIGN_STYLE Automatic
    XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "11.0"
    XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${SEEN_BUNDLE_ID}"
  )

  # example
  add_subdirectory(example/darwin)
  file(GLOB_RECURSE EXAMPLE_SOURCE
    "${seen_example_SOURCE_DIR}/*.h"
    "${seen_example_SOURCE_DIR}/*.swift"
  )
  set_source_files_properties(${EXAMPLE_SOURCE} PROPERTIES COMPILE_FLAGS -fobjc-arc)
  source_group(TREE ${seen_example_SOURCE_DIR} FILES ${EXAMPLE_SOURCE})
  add_executable(seen_example MACOSX_BUNDLE ${EXAMPLE_SOURCE})
  target_link_libraries(seen_example PRIVATE seen)
  set_target_properties(seen_example PROPERTIES
    XCODE_ATTRIBUTE_CODE_SIGN_STYLE Automatic
    XCODE_ATTRIBUTE_ENABLE_PREVIEWS YES
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${SEEN_BUNDLE_ID}.example"
    XCODE_ATTRIBUTE_SWIFT_OBJC_BRIDGING_HEADER "${seen_example_SOURCE_DIR}/BridgingHeaders.h"
    XCODE_ATTRIBUTE_SWIFT_OPTIMIZATION_LEVEL -Onone
  )
endif()
