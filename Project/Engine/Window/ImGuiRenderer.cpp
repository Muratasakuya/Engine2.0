#include "ImGuiRenderer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Engine/Asset/AssetManager.h>
#include <Game/System/RigidBodySystem.h>

//============================================================================*/
//	ImGuiRenderer classMethods
//============================================================================*/

bool ImGuiRenderer::show_ = true;
bool ImGuiRenderer::cameraInfoEnable_ = false;
D3D12_GPU_DESCRIPTOR_HANDLE ImGuiRenderer::renderTextureGPUHandle_ = {};

void ImGuiRenderer::Init(const D3D12_GPU_DESCRIPTOR_HANDLE& renderTextureGPUHandle) {

	// RenderTextureのGpuHandleの取得
	renderTextureGPUHandle_ = renderTextureGPUHandle;

	// Guiに使用するTextureの読み込み
	AssetManager::LoadTexture("GuiIcon");

	AssetManager::LoadTexture("3DGameObjectIcon");

	AssetManager::LoadTexture("ParticleEditIcon");

	AssetManager::LoadTexture("CameraIcon");

	AssetManager::LoadTexture("EditorIcon");

}

void ImGuiRenderer::Render() {
#ifdef _DEBUG

	const ImVec2 mainWindowPos = ImVec2(210.0f, 64.0f);
	const ImVec2 mainWindowSize = ImVec2(768.0f, 432.0f);

	if (!show_) {
		ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(200.0f, 40.0f), ImGuiCond_Always);
		ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		if (ImGui::Button("Display ImGui", ImVec2(180.0f, 24.0f))) {

			show_ = true;
		}

		ImGui::End();
		return;
	}

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(kWindowWidth, kWindowHeight));
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
	ImGui::Begin("Engine", nullptr, windowFlags);

	//* GuiSetting
	GuiSetting(mainWindowPos);
	//* MainWindow
	MainWindowSetting(mainWindowPos);
	//* GameObjectList
	GameObjectList(mainWindowPos);
	//* ParticleList
	ParticleList(mainWindowPos);
	//* CameraList
	CameraList(mainWindowPos);
	//* EditorList
	EditorList(mainWindowPos);
	//* EngineLog
	EngineLog(mainWindowPos, mainWindowSize);
	//* Selected
	SelectedInfo(mainWindowPos, mainWindowSize);
	//* CollisionLog
	CollisionLog(mainWindowPos, mainWindowSize);

	ImGui::End();

#endif // _DEBUG
}

void ImGuiRenderer::GuiSetting(const ImVec2& mainWindowPos) {

	ImGui::SetCursorPos(ImVec2(6.0f, 14.0f));
	D3D12_GPU_DESCRIPTOR_HANDLE guiIconGpuHandle = AssetManager::GetTextureGPUHandle("GuiIcon");
	ImGui::Image(ImTextureID(guiIconGpuHandle.ptr), ImVec2(32.0f, 32.0f));
	ImGui::SetCursorPos(ImVec2(44.0f, 18.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
	if (ImGui::Button("UnDisplay ImGui", ImVec2(160.0f, 22.0f))) {
		show_ = false;
	}
	ImGui::PopStyleVar();

	ImGui::SetCursorPos(ImVec2(mainWindowPos.x, 6.0f));
	ImGui::BeginChild("Information",
		ImVec2(768.0f, 44.0f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::EndChild();

	ImVec2 p1 = ImGui::GetCursorScreenPos();  //* 始点
	p1.y = 56.0f;
	ImVec2 p2 = ImVec2(p1.x + 968.0f, p1.y); //* 終点

	ImGui::GetWindowDrawList()->AddLine(p1, p2, IM_COL32(64, 64, 64, 255), 1.0f);

}

void ImGuiRenderer::MainWindowSetting(const ImVec2& mainWindowPos) {

	const ImVec2 imageSize(768.0f, 432.0f);
	ImGui::SetCursorPos(mainWindowPos);
	ImGui::Image(ImTextureID(renderTextureGPUHandle_.ptr), imageSize);

}

void ImGuiRenderer::GameObjectList(const ImVec2& mainWindowPos) {

	ImGui::SetCursorPos(ImVec2(2.0f, mainWindowPos.y - 6.0f));
	D3D12_GPU_DESCRIPTOR_HANDLE gameObjectIconGpuHandle = AssetManager::GetTextureGPUHandle("3DGameObjectIcon");
	ImGui::Image(ImTextureID(gameObjectIconGpuHandle.ptr), ImVec2(40.0f, 40.0f));

	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(48.0f, mainWindowPos.y + 2.0f));
	ImGui::Text("No GameObject");

}

void ImGuiRenderer::ParticleList(const ImVec2& mainWindowPos) {

	ImGui::SetCursorPos(ImVec2(6.0f, mainWindowPos.y + 64.0f));
	D3D12_GPU_DESCRIPTOR_HANDLE particleIconGpuHandle = AssetManager::GetTextureGPUHandle("ParticleEditIcon");
	ImGui::Image(ImTextureID(particleIconGpuHandle.ptr), ImVec2(32.0f, 32.0f));

	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(48.0f, mainWindowPos.y + 69.0f));
	ImGui::Text("No GameObject");

}

void ImGuiRenderer::CameraList(const ImVec2& mainWindowPos) {

	ImGui::SetCursorPos(ImVec2(6.0f, mainWindowPos.y + 128.0f));
	D3D12_GPU_DESCRIPTOR_HANDLE cameraIconGpuHandle = AssetManager::GetTextureGPUHandle("CameraIcon");
	ImGui::Image(ImTextureID(cameraIconGpuHandle.ptr), ImVec2(32.0f, 32.0f));

	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(44.0f, mainWindowPos.y + 133.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
	if (ImGui::Button("CameraSetting", ImVec2(160.0f, 22.0f))) {

		cameraInfoEnable_ = true;
	}
	ImGui::PopStyleVar();

}

void ImGuiRenderer::EditorList(const ImVec2& mainWindowPos) {

	ImGui::SetCursorPos(ImVec2(6.0f, mainWindowPos.y + 192.0f));
	D3D12_GPU_DESCRIPTOR_HANDLE editorIconGpuHandle = AssetManager::GetTextureGPUHandle("EditorIcon");
	ImGui::Image(ImTextureID(editorIconGpuHandle.ptr), ImVec2(32.0f, 32.0f));

	//* AddEditor
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(48.0f, mainWindowPos.y + 197.0f));
	ImGui::Text("Can't launch editor");

}

void ImGuiRenderer::EngineLog(const ImVec2& mainWindowPos, const ImVec2& mainWindowSize) {

	ImGui::SetCursorPos(ImVec2(8.0f, mainWindowPos.y + mainWindowSize.y + 4.0f));
	ImGui::BeginChild("EngineChild",
		ImVec2((((kWindowWidth / 2.0f) + (mainWindowSize.x / 2.0f) - 254.0f)) / 2.0f, 212.0f),
		true, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::Text("Engine");
	ImGui::Separator();
	ImGui::Text("Frame Rate: %.1f fps", ImGui::GetIO().Framerate); //* フレームレート情報

	ImGui::EndChild();

}

void ImGuiRenderer::SelectedInfo(const ImVec2& mainWindowPos, const ImVec2& mainWindowSize) {

	ImGui::SetCursorPos(ImVec2(mainWindowPos.x + mainWindowSize.x + 6.0f, mainWindowPos.y - 32.0f));
	ImGui::BeginChild("RightPanelChild", ImVec2(288.0f, mainWindowSize.y + 32.0f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::EndChild();

}

void ImGuiRenderer::CollisionLog(const ImVec2& mainWindowPos, const ImVec2& mainWindowSize) {

	ImGui::SetCursorPos(ImVec2(mainWindowPos.x + mainWindowSize.x - 192.0f, mainWindowPos.y + mainWindowSize.y + 4.0f));
	ImGui::BeginChild("CollisionLogsChild", ImVec2(486.0f, 212.0f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
	RigidBodySystem::Inforamtion();

	ImGui::EndChild();

}