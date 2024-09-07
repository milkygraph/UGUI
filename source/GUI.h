#pragma once
#include <opencv2/opencv.hpp>
#include "raylib.h"
#include "imgui.h"
#include "Action.h"
#include "GUIWindow.hpp"


class GUI {
public:
	GUI() = default;
	~GUI() = default;
	void Init();
	void Shutdown();
	void Begin();
	void End();
	static void SubscribeViewportResize(std::function<void(ImVec2)> func);

private:
	static inline Action<ImVec2> m_ViewportResizeAction{};
	std::vector<GUIWindow> m_Windows{};
	ImVec2 m_ViewportSize{};
};