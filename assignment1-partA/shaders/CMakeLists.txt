set(SHADER_SOURCE
    "${ASS01_SHADER_ROOT}/circle.fs.glsl"
    "${ASS01_SHADER_ROOT}/circle.vs.glsl"
    "${ASS01_SHADER_ROOT}/references.fs.glsl"
    "${ASS01_SHADER_ROOT}/references.vs.glsl")

set_source_files_properties(${SHADER_SOURCE} 
    PROPERTIES HEADER_FILE_ONLY TRUE)

set(SHADER_INCLUDE "${ASS01_GENERATED_ROOT}/ShaderPaths.hpp")

configure_file(
    "${ASS01_SHADER_ROOT}/ShaderPaths.hpp.in"
    ${SHADER_INCLUDE})

set(SHADER_LIST
    ${SHADER_SOURCE}
    ${SHADER_INCLUDE}
    PARENT_SCOPE)

