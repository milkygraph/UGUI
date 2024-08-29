#include "Application.hpp"
#include "raylib.h"


Application::Application() {
	InitWindow(m_window_width, m_window_height, m_window_title);

	SetTargetFPS(60);

	m_gui = new GUI();
}

Application::~Application() {
	delete m_gui;
	CloseWindow();
}

void Application::Run() const {
	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(RAYWHITE);

		m_gui->Begin();

		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

		m_gui->End();
		EndDrawing();
	}
}