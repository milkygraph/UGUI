#pragma once
#include "raylib.h"
#include "imgui.h"
#include "Action.h"
#include "GUIWindow.hpp"


class GUI {
public:
	GUI();
	~GUI();
	void Init();
	void Begin();
	void End(RenderTexture2D texture);
	static void SubscribeViewportResize(std::function<void(ImVec2)> func);

private:
	static inline Action<ImVec2> m_ViewportResizeAction{};
	std::vector<GUIWindow> m_Windows{};
	ImVec2 m_ViewportSize{};

};