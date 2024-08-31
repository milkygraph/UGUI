#include "GUI.h"

#include "imgui.h"
#include "imgui_impl_raylib.h"
#include "Application.h"

char* imgui_ini = nullptr;

#if defined(PLATFORM_WEB)
#include <stdio.h>
#include <string.h>
#include <emscripten/fetch.h>
#include <memory>

void downloadSucceeded(emscripten_fetch_t *fetch) {
	printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);
	imgui_ini = (char*)malloc(fetch->numBytes + 1);
	memcpy(imgui_ini, fetch->data, fetch->numBytes);
	// The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
	emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

void downloadFailed(emscripten_fetch_t *fetch) {
	printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
	emscripten_fetch_close(fetch); // Also free data on failure.
}
#endif

GUI::GUI() {}

void GUI::Init() {// Initialize imgui
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui_ImplRaylib_Init();
	Imgui_ImplRaylib_BuildFontAtlas();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Load ini file
#if defined(PLATFORM_WEB)
	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	strcpy(attr.requestMethod, "GET");
	attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
	attr.onsuccess = downloadSucceeded;
	attr.onerror = downloadFailed;
	emscripten_fetch(&attr, "imgui.ini");
	ImGui::LoadIniSettingsFromMemory(imgui_ini, strlen(imgui_ini));
#else
	ImGui::LoadIniSettingsFromDisk("imgui.ini");
#endif

	GUIWindow viewportWindow("ViewportTest");
	viewportWindow.SetUpdateFunction([]() {
		ImGui::Text("Hello, world!");
	});
	m_Windows.push_back(viewportWindow);
}


GUI::~GUI() {
	// Shutdown imgui
	ImGui_ImplRaylib_Shutdown();
}


void GUI::Begin() {
	ImGui_ImplRaylib_NewFrame();
	ImGui_ImplRaylib_ProcessEvents();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
	ImGui::ShowDemoWindow();

	for (auto& window : m_Windows){
		window.Begin();
		window.Update();
	}
}


void GUI::End(RenderTexture2D texture) {
	for (auto& window : m_Windows)
		window.End();

	// Render the viewport texture to a viewport window
	ImGui::Begin("Viewport");
	ImVec2 size = ImGui::GetContentRegionAvail();

	if (size.x != m_ViewportSize.x || size.y != m_ViewportSize.y) {
		m_ViewportSize = size;
		m_ViewportResizeAction.Trigger(m_ViewportSize);
	}

	ImGui::Image(&Application::GetRenderTexture().texture, m_ViewportSize, {0, 1 }, {1, 0 });
	ImGui::End();
	ImGui::Render();
	ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
}


void GUI::SubscribeViewportResize(std::function<void(ImVec2)> func) {
	m_ViewportResizeAction.Subscribe(std::move(func));
}