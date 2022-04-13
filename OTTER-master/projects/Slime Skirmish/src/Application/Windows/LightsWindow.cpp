#include "LightsWindow.h"
#include "../Application.h"
#include "Utils/ImGuiHelper.h"
#include "imgui_internal.h"
#include <set>

LightsWindow::LightsWindow()
	: IEditorWindow()
{
	Name = "Lights";
	SplitDirection = ImGuiDir_::ImGuiDir_None;
	Requirements = EditorWindowRequirements::Window;
	Open = false;
}

LightsWindow::~LightsWindow() = default;

void LightsWindow::Render()
{
	using namespace Gameplay;
	Application& app = Application::Get();

	RenderLayer::Sptr layer = app.GetLayer<RenderLayer>();

	_RenderEffect(layer->GetLightAccumulationShader());

	layer->GetLightAccumulationShader()->SetUniform("u_Diffuse", diffuseStrength);
	layer->GetLightAccumulationShader()->SetUniform("u_Specular", specularStrength);
	//layer->GetLightAccumulationShader()->SetUniform("u_Ambient", ambientStrength);

	//layer->GetLightingBuffer()
}

void LightsWindow::_RenderEffect(const ShaderProgram::Sptr& value)
{
	ImGui::PushID(value.get());

	ImGuiID id = ImGui::GetID(value->GetDebugName().c_str());
	bool isOpen = ImGui::CollapsingHeader(value->GetDebugName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_ClipLabelForTrailingButton);

	if (isOpen) {
		ImGui::Indent();
		LABEL_LEFT(ImGui::SliderFloat, "Diffuse Strength", &diffuseStrength, 0, 1);
		LABEL_LEFT(ImGui::SliderFloat, "Specular Strength", &specularStrength, 0, 1);
		LABEL_LEFT(ImGui::SliderFloat, "Ambient Strength", &ambientStrength, 0, 1);
		ImGui::Unindent();
		ImGui::Separator();
	}

	ImGui::PopID();
}
