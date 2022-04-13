#pragma once
#include "Application/Layers/PostProcessingLayer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture3D.h"

class PixelationEffect : public PostProcessingLayer::Effect {
public:
	MAKE_PTRS(PixelationEffect);

	PixelationEffect();
	virtual ~PixelationEffect();

	virtual void Apply(const Framebuffer::Sptr& gBuffer) override;
	virtual void RenderImGui() override;

	// Inherited from IResource

	PixelationEffect::Sptr FromJson(const nlohmann::json& data);
	virtual nlohmann::json ToJson() const override;

protected:
	ShaderProgram::Sptr _shader;
	float _amount = 1000.0f;
	float _scale = 20.0f;
};