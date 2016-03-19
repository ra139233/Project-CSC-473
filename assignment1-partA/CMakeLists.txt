#================================
# Directory variables.
#================================
set(ASS01_ROOT "${ANIM_SOURCE_DIR}/assignment1-partA")
set(ASS01_INCLUDE_ROOT "${ASS01_ROOT}/include")
set(ASS01_SOURCE_ROOT "${ASS01_ROOT}/source")
set(ASS01_SHADER_ROOT "${ASS01_ROOT}/shaders")
set(ASS01_GENERATED_ROOT "${ASS01_ROOT}/generated")

#================================
# Add subdirectories.
#================================
add_subdirectory(${ASS01_INCLUDE_ROOT})
add_subdirectory(${ASS01_SOURCE_ROOT})
add_subdirectory(${ASS01_SHADER_ROOT})

#================================
# Include directories.
#================================
include_directories(
    ${ATLAS_INCLUDE_DIRS}
    ${ASS01_INCLUDE_ROOT}
    ${ASS01_GENERATED_ROOT})

#================================
# Source groups.
#================================
source_group("include" FILES ${INCLUDE_LIST})
source_group("source" FILES ${SOURCE_LIST})
source_group("shaders" FILES ${SHADER_LIST})

add_executable(assignment1-partA ${SOURCE_LIST} ${INCLUDE_LIST} ${SHADER_LIST})
target_link_libraries(assignment1-partA ${ATLAS_LIBRARIES})

