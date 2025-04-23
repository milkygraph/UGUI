#include <LuaContext.hpp>

class LuaEngine {
public:
    LuaEngine();
    ~LuaEngine();

private:
    LuaCpp::LuaContext m_Context;
};
