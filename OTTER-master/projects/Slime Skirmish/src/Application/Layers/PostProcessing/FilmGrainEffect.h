#pragma once
#include "Application/Layers/PostProcessingLayer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture3D.h"

class FilmGrainEffect : public PostProcessingLayer::Effect {
public:
	MAKE_PTRS(FilmGrainEffect);

	FilmGrainEffect();
	virtual ~FilmGrainEffect();

	virtual void Apply(const Framebuffer::Sptr& gBuffer) override;
	virtual void RenderImGui() override;

	// Inherited from IResource

	FilmGrainEffect::Sptr FromJson(const nlohmann::json& data);
	virtual nlohmann::json ToJson() const override;

protected:
	ShaderProgram::Sptr _shader;
	float _strength = 0.3f;
	float _amount = 30.0f;
	float _scale = 10000.0f;
};