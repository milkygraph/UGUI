#pragma once
#include "GUI.hpp"

class Application {
public:
	Application();
	~Application();
	void Run() const;
	void Update() const;

	GUI* m_gui;
private:
    const int m_window_width = 800;
	const int m_window_height = 450;
	const char* m_window_title = "GYAT";
};
