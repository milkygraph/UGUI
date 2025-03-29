include(FetchContent)

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

# Set variables for later use
set(LUA_INCLUDE_DIR "${lua_SOURCE_DIR}/src" CACHE INTERNAL "")
set(LUA_LIBRARIES "${lua_SOURCE_DIR}/src/liblua.a" CACHE INTERNAL "")
set(LUA_FOUND TRUE CACHE INTERNAL "")

# Manually add LuaCpp if needed
add_subdirectory(${luacpp_SOURCE_DIR}/Source ${luacpp_BINARY_DIR})
