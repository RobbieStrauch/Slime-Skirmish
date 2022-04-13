#include "ColorCorrectionEffectCool.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"

ColorCorrectionEffectCool::ColorCorrectionEffectCool() :
	ColorCorrectionEffectCool(true) { }

ColorCorrectionEffectCool::ColorCorrectionEffectCool(bool defaultLut) :
	PostProcessingLayer::Effect(),
	_shader(nullptr),
	_strength(1.0f),
	Lutc(nullptr)
{
	Name = "Color Correction Cool";
	_format = RenderTargetType::ColorRgb8;

	_shader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
		{ ShaderPartType::Vertex, "shaders/vertex_shaders/fullscreen_quad.glsl" },
		{ ShaderPartType::Fragment, "shaders/fragment_shaders/post_effects/color_correction.glsl" }
	});

	if (defaultLut)
	{
		Lutc = ResourceManager::CreateAsset<Texture3D>("luts/cool.cube");
	}
}

ColorCorrectionEffectCool::~ColorCorrectionEffectCool() = default;

void ColorCorrectionEffectCool::Apply(const Framebuffer::Sptr & gBuffer)
{
	_shader->Bind();
	Lutc->Bind(1);
	_shader->SetUniform("u_Strength", _strength);
}

void ColorCorrectionEffectCool::RenderImGui()
{
	LABEL_LEFT(ImGui::LabelText, "LUT cool", Lutc ? Lutc->GetDebugName().c_str() : "none");
	LABEL_LEFT(ImGui::SliderFloat, "Strength", &_strength, 0, 1);
}

ColorCorrectionEffectCool::Sptr ColorCorrectionEffectCool::FromJson(const nlohmann::json & data)
{
	ColorCorrectionEffectCool::Sptr result = std::make_shared<ColorCorrectionEffectCool>(false);
	result->Enabled = JsonGet(data, "enabled", true);
	result->_strength = JsonGet(data, "strength", result->_strength);
	result->Lutc = ResourceManager::Get<Texture3D>(Guid(data["lut cool"].get<std::string>()));
	//result->Luts = ResourceManager::Get<Texture3D>(Guid(data["lut swamp"].get<std::string>()));
	return result;
}

nlohmann::json ColorCorrectionEffectCool::ToJson() const
{
	return {
		{ "enabled", Enabled },
		{ "lut cool", Lutc != nullptr ? Lutc->GetGUID().str() : "null" },
		{ "strength", _strength }
	};
}