#include "Application.h"
#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

static Application  *app;

void UpdateDrawFrame(void) {
	app->Update();
}

Application::Application() {
	app = this;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(m_WindowWidth, m_WindowHeight, m_window_title);
	m_GUI = new GUI();
	m_GUI->Init();
	m_RenderTexture = LoadRenderTexture(m_WindowWidth, m_WindowHeight);

	GUI::SubscribeViewportResize([this](ImVec2 size) { this->OnViewportResize(size); });
}

Application::~Application() {
    delete m_GUI;
	UnloadRenderTexture(m_RenderTexture);
    CloseWindow();
}

void Application::Run() const {
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        Update();
    }
#endif
}

void Application::Update() const {
    BeginDrawing();
	BeginTextureMode(m_RenderTexture);
    ClearBackground(RAYWHITE);
    app->m_GUI->Begin();

	// Render part
	{
		DrawText("Hello, world!", 10, 10, 20, DARKGRAY);
	}

	EndTextureMode();
	app->m_GUI->End(m_RenderTexture);
    EndDrawing();
}

void Application::OnViewportResize(ImVec2 size) {
	UnloadTexture(m_RenderTexture.texture);
	m_RenderTexture = LoadRenderTexture(size.x, size.y);
}