# === ImGui Setup ===

include(FetchContent)

set(IMGUI_VERSION v1.91.0)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG ${IMGUI_VERSION}-docking
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
)

FetchContent_MakeAvailable(imgui)

# Glob source files and expose them for later use
file(GLOB IMGUI_SOURCE_FILES
    ${imgui_SOURCE_DIR}/*.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# Make it available to parent scope
set(IMGUI_SOURCE_FILES ${IMGUI_SOURCE_FILES} PARENT_SCOPE)
set(IMGUI_INCLUDE_DIRS ${imgui_SOURCE_DIR} PARENT_SCOPE)
