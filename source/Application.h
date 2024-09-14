#pragma once
#include "GUI.h"
#include "raylib.h"
#include "VideoSource.hpp"
#include <torch/script.h>


class Application {
public:
	Application();
	~Application();
	void Run();
	void Update();
	void OnViewportResize(ImVec2);
	void Predict();

	static Application& GetInstance() { return *s_Instance; }
	static RenderTexture2D& GetRenderTexture() { return s_Instance->m_RenderTexture; }

	std::string model_output = "";

private:
	static inline Application* s_Instance = nullptr;
	GUI m_GUI;
    const int m_WindowWidth = 800;
	const int m_WindowHeight = 450;
	const char* m_window_title = "GYAT";
	RenderTexture2D m_RenderTexture;

	torch::jit::script::Module m_Module;
	std::unique_ptr<VideoSource> m_VideoSource;
};