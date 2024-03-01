project(luajit)

set(LUAJIT_OPTION_BUILD_MODE "static")
set(LUAJIT_OPTION_ENABLE_LUA52COMPAT 1)
CPMAddPackage("gh:GameDevDeps/luajit#luajit2/v2.1-20231117")
add_library(luajit ALIAS luajit.liblua-static)
# target_compile_options(luajit.liblua-static PRIVATE -w)
# target_compile_options(luajit.minilua PRIVATE -w)
