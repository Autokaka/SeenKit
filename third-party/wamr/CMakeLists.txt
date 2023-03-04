project(wamr)

CPMAddPackage("gh:bytecodealliance/wasm-micro-runtime#WAMR-1.1.2")

add_library(wamr ALIAS iwasm_static)
