cmake_minimum_required(VERSION 3.27.1) # FetchContent is available in 3.11+
project(GYAT)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set export directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (EMSCRIPTEN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
  set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
endif ()

# Dependencies

# Raylib
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif()
endif()

# ImGui
set(IMGUI_VERSION v1.91.0)
FetchContent_Declare(
        imgui
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.0-docking
)
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(imgui)
endif()

# Collect source files for imgui
file(GLOB IMGUI_SOURCE_FILES
        ${imgui_SOURCE_DIR}/*.h
        ${imgui_SOURCE_DIR}/*.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
include_directories(${PROJECT_NAME} PUBLIC ${imgui_SOURCE_DIR})

# Application source files
file(GLOB SRC_FILES  source/*.cpp source/*.h)

# Create executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${IMGUI_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} raylib)
target_include_directories(${PROJECT_NAME} PUBLIC ${imgui_SOURCE_DIR})

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()