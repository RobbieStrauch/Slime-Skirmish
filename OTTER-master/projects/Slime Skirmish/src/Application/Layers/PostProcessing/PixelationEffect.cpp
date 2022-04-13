#include "PixelationEffect.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"

PixelationEffect::PixelationEffect() :
	PostProcessingLayer::Effect(),
	_shader(nullptr)
{
	Name = "Pixelation Effect";
	_format = RenderTargetType::ColorRgb8;

	_shader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
		{ ShaderPartType::Vertex, "shaders/vertex_shaders/fullscreen_quad.glsl" },
		{ ShaderPartType::Fragment, "shaders/fragment_shaders/post_effects/pixelation.glsl" }
	});
}

PixelationEffect::~PixelationEffect() = default;

void PixelationEffect::Apply(const Framebuffer::Sptr & gBuffer)
{
	_shader->Bind();
	gBuffer->BindAttachment(RenderTargetAttachment::Depth, 1);
	gBuffer->BindAttachment(RenderTargetAttachment::Color1, 2); // The normal buffer
	_shader->SetUniform("u_Amount", _amount);
	_shader->SetUniform("u_Scale", _scale);
}

void PixelationEffect::RenderImGui()
{
	LABEL_LEFT(ImGui::SliderFloat, "Amount", &_amount, 1, 10000);
	LABEL_LEFT(ImGui::SliderFloat, "Scale", &_scale, 1, 100);
}

PixelationEffect::Sptr PixelationEffect::FromJson(const nlohmann::json & data)
{
	PixelationEffect::Sptr result = std::make_shared<PixelationEffect>();
	result->Enabled = JsonGet(data, "enabled", true);
	result->_amount = JsonGet(data, "amount", result->_amount);
	result->_scale = JsonGet(data, "scale", result->_scale);
	return result;
}

nlohmann::json PixelationEffect::ToJson() const
{
	return
	{
		{ "enabled", Enabled },
		{ "amount", _amount },
		{ "scale", _scale }
	};
}