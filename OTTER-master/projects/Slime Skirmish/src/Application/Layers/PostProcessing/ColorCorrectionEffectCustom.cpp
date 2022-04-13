#include "ColorCorrectionEffectCustom.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"

ColorCorrectionEffectCustom::ColorCorrectionEffectCustom() :
	ColorCorrectionEffectCustom(true) { }

ColorCorrectionEffectCustom::ColorCorrectionEffectCustom(bool defaultLut) :
	PostProcessingLayer::Effect(),
	_shader(nullptr),
	_strength(1.0f),
	Lutcu(nullptr)
{
	Name = "Color Correction Custom";
	_format = RenderTargetType::ColorRgb8;

	_shader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
		{ ShaderPartType::Vertex, "shaders/vertex_shaders/fullscreen_quad.glsl" },
		{ ShaderPartType::Fragment, "shaders/fragment_shaders/post_effects/color_correction.glsl" }
	});

	if (defaultLut)
	{
		Lutcu = ResourceManager::CreateAsset<Texture3D>("luts/purple.cube");
	}
}

ColorCorrectionEffectCustom::~ColorCorrectionEffectCustom() = default;

void ColorCorrectionEffectCustom::Apply(const Framebuffer::Sptr & gBuffer)
{
	_shader->Bind();
	Lutcu->Bind(1);
	_shader->SetUniform("u_Strength", _strength);
}

void ColorCorrectionEffectCustom::RenderImGui()
{
	LABEL_LEFT(ImGui::LabelText, "LUT custom", Lutcu ? Lutcu->GetDebugName().c_str() : "none");
	LABEL_LEFT(ImGui::SliderFloat, "Strength", &_strength, 0, 1);
}

ColorCorrectionEffectCustom::Sptr ColorCorrectionEffectCustom::FromJson(const nlohmann::json & data)
{
	ColorCorrectionEffectCustom::Sptr result = std::make_shared<ColorCorrectionEffectCustom>(false);
	result->Enabled = JsonGet(data, "enabled", true);
	result->_strength = JsonGet(data, "strength", result->_strength);
	result->Lutcu = ResourceManager::Get<Texture3D>(Guid(data["lut custom"].get<std::string>()));
	//result->Luts = ResourceManager::Get<Texture3D>(Guid(data["lut swamp"].get<std::string>()));
	return result;
}

nlohmann::json ColorCorrectionEffectCustom::ToJson() const
{
	return {
		{ "enabled", Enabled },
		{ "lut custom", Lutcu != nullptr ? Lutcu->GetGUID().str() : "null" },
		{ "strength", _strength }
	};
}