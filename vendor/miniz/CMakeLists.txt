project(miniz)

CPMAddPackage("gh:richgel999/miniz#3.0.2")

target_compile_options(miniz PRIVATE -w)
