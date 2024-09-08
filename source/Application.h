#pragma once
#include "GUI.h"
#include "raylib.h"
#include "VideoSource.hpp"


class Application {
public:
	Application();
	~Application();
	void Run();
	void Update();
	void OnViewportResize(ImVec2);

	static Application& GetInstance() { return *s_Instance; }
	static RenderTexture2D& GetRenderTexture() { return s_Instance->m_RenderTexture; }

private:
	static inline Application* s_Instance = nullptr;
	GUI m_GUI;
    const int m_WindowWidth = 800;
	const int m_WindowHeight = 450;
	const char* m_window_title = "GYAT";
	RenderTexture2D m_RenderTexture;

	std::unique_ptr<VideoSource> m_VideoSource;
};