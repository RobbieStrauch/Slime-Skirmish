#include "ColorCorrectionEffectWarm.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"

ColorCorrectionEffectWarm::ColorCorrectionEffectWarm() :
	ColorCorrectionEffectWarm(true) { }

ColorCorrectionEffectWarm::ColorCorrectionEffectWarm(bool defaultLut) :
	PostProcessingLayer::Effect(),
	_shader(nullptr),
	_strength(1.0f),
	Lutw(nullptr)
{
	Name = "Color Correction Warm";
	_format = RenderTargetType::ColorRgb8;

	_shader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
		{ ShaderPartType::Vertex, "shaders/vertex_shaders/fullscreen_quad.glsl" },
		{ ShaderPartType::Fragment, "shaders/fragment_shaders/post_effects/color_correction.glsl" }
	});

	if (defaultLut)
	{
		Lutw = ResourceManager::CreateAsset<Texture3D>("luts/warm.cube");
	}
}

ColorCorrectionEffectWarm::~ColorCorrectionEffectWarm() = default;

void ColorCorrectionEffectWarm::Apply(const Framebuffer::Sptr & gBuffer)
{
	_shader->Bind();
	Lutw->Bind(1);
	_shader->SetUniform("u_Strength", _strength);
}

void ColorCorrectionEffectWarm::RenderImGui()
{
	LABEL_LEFT(ImGui::LabelText, "LUT warm", Lutw ? Lutw->GetDebugName().c_str() : "none");
	LABEL_LEFT(ImGui::SliderFloat, "Strength", &_strength, 0, 1);
}

ColorCorrectionEffectWarm::Sptr ColorCorrectionEffectWarm::FromJson(const nlohmann::json & data)
{
	ColorCorrectionEffectWarm::Sptr result = std::make_shared<ColorCorrectionEffectWarm>(false);
	result->Enabled = JsonGet(data, "enabled", true);
	result->_strength = JsonGet(data, "strength", result->_strength);
	result->Lutw = ResourceManager::Get<Texture3D>(Guid(data["lut warm"].get<std::string>()));
	//result->Luts = ResourceManager::Get<Texture3D>(Guid(data["lut swamp"].get<std::string>()));
	return result;
}

nlohmann::json ColorCorrectionEffectWarm::ToJson() const
{
	return {
		{ "enabled", Enabled },
		{ "lut warm", Lutw != nullptr ? Lutw->GetGUID().str() : "null" },
		{ "strength", _strength }
	};
}