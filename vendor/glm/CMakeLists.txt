project(glm)

CPMAddPackage("gh:g-truc/glm#0.9.9.8")

target_compile_options(glm INTERFACE -w)
