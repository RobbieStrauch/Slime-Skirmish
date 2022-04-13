#include "ToonShadingEffect.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"

ToonShadingEffect::ToonShadingEffect() :
	PostProcessingLayer::Effect(),
	_shader(nullptr),
	_OutlineColor(glm::vec4(1.0, 0.5, 0.5, 1.0)),
	_depthThreshold(0.1f),
	_normalThreshold(0.4f),
	_depthNormalThreshold(0.4f),
	_depthNormalThresholdScale(4),
	_pixelsize(0.7,0.8)
{
	Name = "Toon Shading Effect";
	_format = RenderTargetType::ColorRgb8;

	_shader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
		{ ShaderPartType::Vertex, "shaders/vertex_shaders/fullscreen_quad.glsl" },
		{ ShaderPartType::Fragment, "shaders/fragment_shaders/post_effects/toon_shading.glsl" }
	});
}

ToonShadingEffect::~ToonShadingEffect() = default;

void ToonShadingEffect::Apply(const Framebuffer::Sptr & gBuffer)
{
	_shader->Bind();
	_shader->SetUniform("u_OutlineColor", _OutlineColor);
	_shader->SetUniform("u_DepthThreshold", _depthThreshold);
	_shader->SetUniform("u_NormalThreshold", _normalThreshold);
	_shader->SetUniform("u_DepthNormThreshold", _depthNormalThreshold);
	_shader->SetUniform("u_DepthNormThresholdScale", _depthNormalThresholdScale);
	_shader->SetUniform("u_PixelSize", glm::vec2(1.0f) / (glm::vec2)gBuffer->GetSize());
	gBuffer->BindAttachment(RenderTargetAttachment::Depth, 1);
	gBuffer->BindAttachment(RenderTargetAttachment::Color1, 2); // The normal buffer
}

void ToonShadingEffect::RenderImGui()
{
}

ToonShadingEffect::Sptr ToonShadingEffect::FromJson(const nlohmann::json & data)
{
	ToonShadingEffect::Sptr result = std::make_shared<ToonShadingEffect>();
	result->Enabled = JsonGet(data, "enabled", true);
	return result;
}

nlohmann::json ToonShadingEffect::ToJson() const
{
	return
	{
		{ "enabled", Enabled },
	};
}