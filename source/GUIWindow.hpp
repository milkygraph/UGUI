#pragma once
#include "imgui.h"
#include <functional>


class GUIWindow {
public:
	GUIWindow(const char* name, ImGuiWindowFlags flags = 0) : m_Name(name), m_Flags(flags) {}
	~GUIWindow() = default;
	void Begin();
	void Update();
	void End();
	void SetUpdateFunction(std::function<void()> func);

private:
	const char* m_Name;
    ImGuiWindowFlags m_Flags = 0;
	std::function<void()> m_UpdateFunction = []() {};
};
