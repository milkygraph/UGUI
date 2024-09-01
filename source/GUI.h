#pragma once
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
	void End(RenderTexture2D texture);
	static void SubscribeViewportResize(std::function<void(ImVec2)> func);

private:
	static inline Action<ImVec2> m_ViewportResizeAction{};
	std::vector<GUIWindow> m_Windows{};
	ImVec2 m_ViewportSize{};

};