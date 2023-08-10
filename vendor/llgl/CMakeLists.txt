project(llgl)

set(LLGL_BUILD_STATIC_LIB ON)

CPMAddPackage("gh:LukasBanana/LLGL#master")

add_library(llgl ALIAS LLGL)
