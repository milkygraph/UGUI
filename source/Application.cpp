#include <stdexcept>
#include "Application.h"
#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

Image MatToRaylibImage(const cv::Mat& mat) {
	int width = mat.cols;
	int height = mat.rows;
	int channels = mat.channels();

	// Convert OpenCV BGR format to raylib's RGB format
	cv::Mat mat_rgb;
	cv::cvtColor(mat, mat_rgb, cv::COLOR_BGR2RGB);

	// Allocate the raylib Image
	Image image = {
			.data = (void*)RL_MALLOC(width * height * channels),
			.width = width,
			.height = height,
			.mipmaps = 1,
			.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8
	};

	// Copy data from OpenCV Mat to raylib Image
	memcpy(image.data, mat_rgb.data, width * height * channels);

	return image;
}


void UpdateDrawFrame(void) {
	Application::GetInstance().Update();
}

Application::Application() {
	if (s_Instance)
		throw std::runtime_error("Application already exists");
	s_Instance = this;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(m_WindowWidth, m_WindowHeight, m_window_title);
	m_GUI.Init();
	m_RenderTexture = LoadRenderTexture(m_WindowWidth, m_WindowHeight);

	GUI::SubscribeViewportResize([this](ImVec2 size) { this->OnViewportResize(size); });
}

Application::~Application() {
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
		static cv::VideoCapture cap(0);
		static cv::Mat frame;
		static int frameCounter = 0;
		static Image webcamImage;
		static Texture2D webcamTexture;

		frameCounter++;
		if (frameCounter % 5 == 0 || frameCounter == 1) {
			cap >> frame;
			if (!frame.empty()) {
				// Flip the image
				cv::flip(frame, frame, 1);
				webcamImage = MatToRaylibImage(frame);
				if (webcamTexture.id == 0)
					webcamTexture = LoadTextureFromImage(webcamImage);
				UpdateTexture(webcamTexture, webcamImage.data);
				UnloadImage(webcamImage);
			}
		}

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