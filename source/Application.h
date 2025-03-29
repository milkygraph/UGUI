#pragma once
#include "GUI.h"
#include "LuaContext.hpp"
#include "raylib.h"


class Application {
public:
	Application();
	~Application();
	void Run() const;
	void Update() const;
	void OnViewportResize(ImVec2);
    void Lua(const char* code);

	static Application& GetInstance() { return *s_Instance; }
	static RenderTexture2D& GetRenderTexture() { return s_Instance->m_RenderTexture; }

private:
	static inline Application* s_Instance = nullptr;
    LuaCpp::LuaContext lua;
	GUI* m_GUI;
    const int m_WindowWidth = 800;
	const int m_WindowHeight = 450;
	const char* m_window_title = "GYAT";
	RenderTexture2D m_RenderTexture;
};
