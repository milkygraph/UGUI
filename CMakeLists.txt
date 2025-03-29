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

# ----------------------------------------------------
# External Dependencies via FetchContent
# ----------------------------------------------------
include(FetchContent)

# --- Raylib ---
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    endif()
endif()

# --- ImGui ---
set(IMGUI_VERSION v1.91.0)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.0-docking
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
)
FetchContent_MakeAvailable(imgui)

file(GLOB IMGUI_SOURCE_FILES
    ${imgui_SOURCE_DIR}/*.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# --- Lua ---
FetchContent_Declare(
    lua
    URL https://www.lua.org/ftp/lua-5.4.7.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# --- LuaCpp ---
FetchContent_Declare(
    luacpp
    GIT_REPOSITORY https://github.com/milkygraph/luacpp.git
    GIT_TAG v0.2.0
)
FetchContent_MakeAvailable(lua luacpp)

# Build Lua manually
if (EMSCRIPTEN)
    add_custom_target(build_lua
        COMMAND ${CMAKE_COMMAND} -E chdir ${lua_SOURCE_DIR}/src
                emmake make posix CC=emcc
        COMMENT "Building Lua with Emscripten"
    )
else()
    add_custom_target(build_lua
        COMMAND ${CMAKE_COMMAND} -E chdir ${lua_SOURCE_DIR}/src
                make generic
        COMMENT "Building Lua natively"
    )
endif()

# Set Lua paths manually for LuaCpp
set(LUA_INCLUDE_DIR "${lua_SOURCE_DIR}/src")
set(LUA_LIBRARIES "${lua_SOURCE_DIR}/src/liblua.a")
set(LUA_FOUND TRUE)

# Add LuaCpp manually as subdirectory
add_subdirectory(${luacpp_SOURCE_DIR}/Source ${luacpp_BINARY_DIR})

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

# macOS frameworks
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
