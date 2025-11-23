cmake_minimum_required(VERSION 3.22)
project(Emma LANGUAGES C CXX)

# ----------------------------
# Toolchain & Standards
# ----------------------------
# Use the commented-out block if you want manual toolchain setup:
if(NOT EMSCRIPTEN)
    set(CMAKE_TOOLCHAIN_FILE "/path/to/emscripten/platform/Emscripten.cmake")
endif()
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# ----------------------------
# Engine Integration
# ----------------------------
set(ENGINE_NAME nkengine)
set(ENGINE_PATH "../${ENGINE_NAME}")
# We will place the engine build files inside 'build/engine_build' 
# relative to the current binary directory (which is typically 'build/').
add_subdirectory(${ENGINE_PATH} ${ENGINE_NAME}_build)

# ----------------------------
# Project Sources (Only files belonging to this executable)
# ----------------------------
set(SOURCE_PATH "src")
file(GLOB_RECURSE PROJECT_C_SRC "${SOURCE_PATH}/*.c")
file(GLOB_RECURSE PROJECT_CXX_SRC "${SOURCE_PATH}/*.cpp")

# ----------------------------
# Executable Definition
# ----------------------------
add_executable(${PROJECT_NAME}
    ${PROJECT_C_SRC}
    ${PROJECT_CXX_SRC}
)

# Link the executable to the engine library
target_link_libraries(${PROJECT_NAME} PRIVATE ${ENGINE_NAME})

# Equivalent to -I
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/vendor")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src")

# ----------------------------
# WASM/Linker Flags (These are specific to the executable build)
# ----------------------------
set(WASM_FLAGS
    -sUSE_WEBGPU=1
    -sWASM=1
    -sNO_EXIT_RUNTIME=1
    -s\"EXPORTED_RUNTIME_METHODS=['ccall']\"
    -sEXPORTED_FUNCTIONS=['_main']
)

set(DEV_FLAGS
    -sALLOW_MEMORY_GROWTH=1
    -sMAXIMUM_MEMORY=1073741824
    -sINITIAL_MEMORY=67108864
)

set(ASAN
    -fsanitize=address
    -fsanitize=undefined
    -ferror-limit=0
    -g
)

# Apply flags
target_compile_options(${PROJECT_NAME} PRIVATE ${ASAN})
target_link_options(${PROJECT_NAME} PRIVATE ${WASM_FLAGS} ${DEV_FLAGS} ${ASAN})

# ----------------------------
# Preload files
# ----------------------------
file(GLOB_RECURSE PROJECT_PICTURES "${SOURCE_PATH}/*.jpg")
set(${PROJECT_NAME}_PRELOAD_FILES "")

foreach(entry IN LISTS PROJECT_PICTURES)
    # Extract filename
    get_filename_component(fname "${entry}" NAME)

    # Build relative path
    file(RELATIVE_PATH RELATIVE_FILE_PATH 
        "${CMAKE_BINARY_DIR}" 
        "${entry}"
    )

    # Build preload argument: real@virtual
    set(preload_arg "${RELATIVE_FILE_PATH}@textures/${fname}")

    # Append to list
    list(APPEND ${PROJECT_NAME}_PRELOAD_FILES "--preload-file")
    list(APPEND ${PROJECT_NAME}_PRELOAD_FILES "${preload_arg}")
endforeach()

# Fetch Preload files from engine
string(JOIN " " JOIN_PRELOAD ${${PROJECT_NAME}_PRELOAD_FILES})
set(PRELOAD_FLAGS_STRING 
    "${${ENGINE_NAME}_PRELOAD_FLAGS} ${JOIN_PRELOAD}"
)


# The "old school" method of setting link flags (required for --preload-file compatibility)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${PRELOAD_FLAGS_STRING}")

# Ensure the executable rebuilds when the shaders change (LINK_DEPENDS)
# NOTE: The variable holding the engine's generated files list is still correct 
# because it was created in the engine's CMakeLists.txt scope and is accessible here.
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY LINK_DEPENDS ${${ENGINE_NAME}_WGSL_OUT_FILES})

