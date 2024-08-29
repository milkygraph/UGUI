#include "GUI.hpp"
#include "imgui.h"
#include "imgui_impl_raylib.h"
#include "rlImGui.h"

GUI::GUI() {
	// Initialize imgui
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui_ImplRaylib_Init();
	Imgui_ImplRaylib_BuildFontAtlas();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
}


GUI::~GUI() {
	// Shutdown imgui
	ImGui_ImplRaylib_Shutdown();
}


void GUI::Begin() const{
	ImGui_ImplRaylib_NewFrame();
	ImGui_ImplRaylib_ProcessEvents();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
}


void GUI::End() const{
	ImGui::Render();
	ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
}