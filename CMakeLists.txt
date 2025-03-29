cmake_minimum_required(VERSION 3.27.1)
project(UGUI)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Emscripten settings
if (EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s FETCH=1 -s USE_GLFW=3 -s WASM=1 -s ASYNCIFY")
endif()

# macOS frameworks
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

# Include modular setups
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(LuaSetup)
include(ImGUISetup)
include(RaylibSetup)

# ----------------------------------------------------
# Application Source Files
# ----------------------------------------------------
file(GLOB SRC_FILES source/*.cpp source/*.h)

# Create executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${IMGUI_SOURCE_FILES})

# Dependencies
add_dependencies(${PROJECT_NAME} build_lua luacpp)

# Includes
target_include_directories(${PROJECT_NAME} PUBLIC
    ${imgui_SOURCE_DIR}
    ${lua_SOURCE_DIR}/src
    ${luacpp_SOURCE_DIR}/Source
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    raylib
    ${lua_SOURCE_DIR}/src/liblua.a
    luacpp
)
