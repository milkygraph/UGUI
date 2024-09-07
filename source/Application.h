#pragma once
#include "GUI.h"
#include "raylib.h"


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
	void UpdateWebcam();
	void UpdateWebcamTexture();

	static inline Application* s_Instance = nullptr;
	GUI m_GUI;
    const int m_WindowWidth = 800;
	const int m_WindowHeight = 450;
	const char* m_window_title = "GYAT";
	RenderTexture2D m_RenderTexture;

	cv::VideoCapture cap;
	cv::Mat frame;
	Image webcamImage;
	Texture2D webcamTexture;
};