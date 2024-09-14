#include <stdexcept>
#include "Application.h"
#include "raylib.h"
#include "Webcam.hpp"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif


void UpdateDrawFrame(void) {
	Application::GetInstance().Update();
}

Application::Application() {
	if (s_Instance)
		throw std::runtime_error("Application already exists");
	s_Instance = this;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(m_WindowWidth, m_WindowHeight, m_window_title);
	SetTargetFPS(60);
	m_GUI.Init();
	m_RenderTexture = LoadRenderTexture(m_WindowWidth, m_WindowHeight);

	GUI::SubscribeViewportResize([this](ImVec2 size) { this->OnViewportResize(size); });

#if defined(PLATFORM_WEB)
	// TODO: ADD VIDEO SOURCE FOR WEB
#else
	m_VideoSource = std::make_unique<Webcam>();
#endif

	m_VideoSource->StartCapture();


	// Load AI model
	try {
		m_Module = torch::jit::load("./data/detection_model/yolov10_trained.torchscript");
		m_Module.eval();
	} catch (const c10::Error& e) {
		std::cerr << "Error loading the model\n" << e.what() << std::endl;
	}
}

Application::~Application() {
	m_VideoSource->StopCapture();
	m_GUI.Shutdown();
	UnloadRenderTexture(m_RenderTexture);
    CloseWindow();
}

void Application::Run() {
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(120);
    while (!WindowShouldClose()) {
        Update();
    }
#endif
}

void Application::Update() {
    BeginDrawing();
	BeginTextureMode(m_RenderTexture);
    ClearBackground(RAYWHITE);
    m_GUI.Begin();

	// Render part
	{
		Texture2D webcamTexture = m_VideoSource->GetTexture();
		DrawTexture(
				webcamTexture,
				0, 0,
				WHITE
		);
	}

	EndTextureMode();
	m_GUI.End();
    EndDrawing();
}

void Application::OnViewportResize(ImVec2 size) {
	UnloadTexture(m_RenderTexture.texture);
	m_RenderTexture = LoadRenderTexture((int)size.x, (int)size.y);
}


// Function to process YOLO output
void ProcessYOLOOutput(const torch::Tensor& output) {
	// TODO: Implement this function
}

void Application::Predict() {
	Texture2D texture = m_VideoSource->GetTexture();
	constexpr int width = 480;
	constexpr int height = 480;

	// Convert to tensor
	cv::Mat frame = m_VideoSource->GetFrame();
	cv::Mat resized;
	cv::resize(frame, resized, cv::Size(width, height));

	cv::Mat rgb;
	cv::cvtColor(resized, rgb, cv::COLOR_BGR2RGB);

	torch::Tensor tensor_image = torch::from_blob(rgb.data, {1, height, width, 3}, torch::kByte);
	tensor_image = tensor_image.permute({0, 3, 1, 2});
	tensor_image = tensor_image.toType(torch::kFloat);
	tensor_image = tensor_image.div(255);

	// Predict
	auto output = m_Module.forward({tensor_image}).toTensor();
	std::cout << output.sizes() << std::endl;

	// Get the output
	ProcessYOLOOutput(output);
}