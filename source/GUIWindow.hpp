#pragma once
#include <functional>


class GUIWindow {
public:
	GUIWindow(const char* name) : m_Name(name) {}
	~GUIWindow() = default;
	void Begin();
	void Update();
	void End();
	void SetUpdateFunction(std::function<void()> func);

private:
	const char* m_Name;
	std::function<void()> m_UpdateFunction = []() {};
};