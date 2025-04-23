#include "GUIWindow.hpp"
#include <imgui.h>


void GUIWindow::Begin() {
	ImGui::Begin(m_Name, NULL, m_Flags);
}

void GUIWindow::End() {
	ImGui::End();
}

void GUIWindow::Update() {
	m_UpdateFunction();
}

void GUIWindow::SetUpdateFunction(std::function<void()> func) {
	m_UpdateFunction = std::move(func);
}
