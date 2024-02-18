project(scriptx)
cmake_policy(SET CMP0079 NEW)

set(SCRIPTX_BACKEND Lua)

CPMAddPackage("gh:walterschell/Lua#master")
CPMAddPackage("gh:Tencent/ScriptX#main")

add_library(scriptx ALIAS ScriptX)
target_compile_definitions(ScriptX PUBLIC SCRIPTX_BACKEND_TRAIT_PREFIX=../backend/Lua/trait/Trait)
target_link_libraries(ScriptX PUBLIC lua_static)
target_compile_options(ScriptX PRIVATE -w)
