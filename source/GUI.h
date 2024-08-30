#pragma once
#include "raylib.h"
#include "imgui.h"
#include "Action.h"

class GUI {
public:
	GUI();
	~GUI();
	void Begin();
	void End(RenderTexture2D texture);
	static void SubscribeViewportResize(std::function<void(ImVec2)> func);
private:
	static inline Action<ImVec2> m_ViewportResizeAction{};
	ImVec2 m_ViewportSize{};
};