#include "Application.hpp"
#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

static Application* app = nullptr;

void UpdateDrawFrame(void) {
	app->Update();
}

Application::Application() {
    app = this;
    InitWindow(m_window_width, m_window_height, m_window_title);
    m_gui = new GUI();
}

Application::~Application() {
    delete m_gui;
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

    ClearBackground(RAYWHITE);

    app->m_gui->Begin();

    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    app->m_gui->End();
    EndDrawing();
}