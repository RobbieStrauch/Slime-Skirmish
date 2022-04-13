#include "NightVisionEffect.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"

NightVisionEffect::NightVisionEffect() :
	PostProcessingLayer::Effect(),
	_shader(nullptr)
{
	Name = "Night Vision Effect";
	_format = RenderTargetType::ColorRgb8;

	_shader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
		{ ShaderPartType::Vertex, "shaders/vertex_shaders/fullscreen_quad.glsl" },
		{ ShaderPartType::Fragment, "shaders/fragment_shaders/post_effects/Night_vision.glsl" }
	});
}

NightVisionEffect::~NightVisionEffect() = default;

void NightVisionEffect::Apply(const Framebuffer::Sptr & gBuffer)
{
	_shader->Bind();
	gBuffer->BindAttachment(RenderTargetAttachment::Depth, 1);
	gBuffer->BindAttachment(RenderTargetAttachment::Color1, 2); // The normal buffer
}

void NightVisionEffect::RenderImGui()
{
}

NightVisionEffect::Sptr NightVisionEffect::FromJson(const nlohmann::json & data)
{
	NightVisionEffect::Sptr result = std::make_shared<NightVisionEffect>();
	result->Enabled = JsonGet(data, "enabled", true);
	return result;
}

nlohmann::json NightVisionEffect::ToJson() const
{
	return
	{
		{ "enabled", Enabled },
	};
}