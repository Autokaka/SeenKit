if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(SEEN_BUILD_TYPE "Debug")
  set(SEEN_BUILD_DEBUG ON)
  set(SEEN_COMPILE_FLAGS ${SEEN_COMPILE_FLAGS} SEEN_BUILD_DEBUG=1)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
  set(SEEN_BUILD_TYPE "Profile")
  set(SEEN_BUILD_PROFILE ON)
  set(SEEN_BUILD_NDEBUG ON)
  set(SEEN_COMPILE_FLAGS ${SEEN_COMPILE_FLAGS} SEEN_BUILD_NDEBUG=1)
  set(SEEN_COMPILE_FLAGS ${SEEN_COMPILE_FLAGS} SEEN_BUILD_PROFILE=1)
else()
  set(SEEN_BUILD_TYPE "Release")
  set(SEEN_BUILD_RELEASE ON)
  set(SEEN_BUILD_NDEBUG ON)
  set(SEEN_COMPILE_FLAGS ${SEEN_COMPILE_FLAGS} SEEN_BUILD_NDEBUG=1)
  set(SEEN_COMPILE_FLAGS ${SEEN_COMPILE_FLAGS} SEEN_BUILD_RELEASE=1)
endif()

if(APPLE)
  set(SEEN_BUILD_DARWIN ON)
  set(SEEN_COMPILE_FLAGS ${SEEN_COMPILE_FLAGS} SEEN_BUILD_DARWIN=1)
  if(IOS)
    set(SEEN_BUILD_IOS ON)
    set(SEEN_OS_NAME "iOS")
    set(SEEN_COMPILE_FLAGS ${SEEN_COMPILE_FLAGS} SEEN_BUILD_IOS=1)
  else()
    set(SEEN_BUILD_MACOS ON)
    set(SEEN_OS_NAME "macOS")
    set(SEEN_COMPILE_FLAGS ${SEEN_COMPILE_FLAGS} SEEN_BUILD_MACOS=1)
  endif()
endif()

if(ANDROID)
  set(SEEN_BUILD_ANDROID ON)
  set(SEEN_OS_NAME "Android")
  set(SEEN_COMPILE_FLAGS ${SEEN_COMPILE_FLAGS} SEEN_BUILD_ANDROID=1)
endif()

if(WIN32)
  set(SEEN_BUILD_WINDOWS ON)
  set(SEEN_OS_NAME "Windows")
  set(SEEN_COMPILE_FLAGS ${SEEN_COMPILE_FLAGS} SEEN_BUILD_WINDOWS=1)
endif()

if(UNIX AND NOT APPLE)
  set(SEEN_BUILD_LINUX ON)
  set(SEEN_OS_NAME "Linux")
  set(SEEN_COMPILE_FLAGS ${SEEN_COMPILE_FLAGS} SEEN_BUILD_LINUX=1)
endif()

set(SEEN_BUNDLE_ID graphics.seen)
set(SEEN_VERSION "SeenKit-v${PROJECT_VERSION}-${SEEN_OS_NAME}-${SEEN_BUILD_TYPE}")
set(SEEN_COMPILE_FLAGS ${SEEN_COMPILE_FLAGS} SEEN_VERSION=${SEEN_VERSION})
