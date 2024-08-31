#include "GUIWindow.hpp"
#include <imgui.h>


void GUIWindow::Begin() {
	ImGui::Begin(m_Name);
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