#include "FilmGrainEffect.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"

FilmGrainEffect::FilmGrainEffect() :
	PostProcessingLayer::Effect(),
	_shader(nullptr)
{
	Name = "Film Grain Effect";
	_format = RenderTargetType::ColorRgb8;

	_shader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
		{ ShaderPartType::Vertex, "shaders/vertex_shaders/fullscreen_quad.glsl" },
		{ ShaderPartType::Fragment, "shaders/fragment_shaders/post_effects/film_grain.glsl" }
	});
}

FilmGrainEffect::~FilmGrainEffect() = default;

void FilmGrainEffect::Apply(const Framebuffer::Sptr & gBuffer)
{
	_shader->Bind();
	gBuffer->BindAttachment(RenderTargetAttachment::Depth, 1);
	gBuffer->BindAttachment(RenderTargetAttachment::Color1, 2); // The normal buffer
	_shader->SetUniform("u_Strength", _strength);
	_shader->SetUniform("u_Amount", _amount);
	_shader->SetUniform("u_Scale", _scale);
}

void FilmGrainEffect::RenderImGui()
{
	LABEL_LEFT(ImGui::SliderFloat, "Strength", &_strength, 0, 1);
	LABEL_LEFT(ImGui::SliderFloat, "Amount", &_amount, 0, 50);
	LABEL_LEFT(ImGui::SliderFloat, "Scale", &_scale, 1, 10000);
}

FilmGrainEffect::Sptr FilmGrainEffect::FromJson(const nlohmann::json & data)
{
	FilmGrainEffect::Sptr result = std::make_shared<FilmGrainEffect>();
	result->Enabled = JsonGet(data, "enabled", true);
	result->_strength = JsonGet(data, "strength", result->_strength);
	result->_amount = JsonGet(data, "amount", result->_amount);
	result->_scale = JsonGet(data, "scale", result->_scale);
	return result;
}

nlohmann::json FilmGrainEffect::ToJson() const
{
	return
	{
		{ "enabled", Enabled },
		{ "strength", _strength },
		{ "amount", _amount },
		{ "scale", _scale }
	};
}