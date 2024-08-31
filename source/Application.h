#pragma once
#include "GUI.h"
#include "raylib.h"


class Application {
public:
	Application();
	~Application();
	void Run() const;
	void Update() const;
	void OnViewportResize(ImVec2);

	static RenderTexture2D GetRenderTexture() { return s_Instance->m_RenderTexture; }

private:
	static inline Application* s_Instance = nullptr;
	GUI* m_GUI;
    const int m_WindowWidth = 800;
	const int m_WindowHeight = 450;
	const char* m_window_title = "GYAT";
	RenderTexture2D m_RenderTexture;
};