project(wamr)

CPMAddPackage("gh:bytecodealliance/wasm-micro-runtime#WAMR-1.2.2")

add_library(wamr ALIAS iwasm_static)

target_compile_options(iwasm_static PRIVATE -w)
