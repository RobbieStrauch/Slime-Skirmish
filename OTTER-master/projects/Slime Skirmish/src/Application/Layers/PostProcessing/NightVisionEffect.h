#pragma once
#include "Application/Layers/PostProcessingLayer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture3D.h"

class NightVisionEffect : public PostProcessingLayer::Effect {
public:
	MAKE_PTRS(NightVisionEffect);

	NightVisionEffect();
	virtual ~NightVisionEffect();

	virtual void Apply(const Framebuffer::Sptr& gBuffer) override;
	virtual void RenderImGui() override;

	// Inherited from IResource

	NightVisionEffect::Sptr FromJson(const nlohmann::json& data);
	virtual nlohmann::json ToJson() const override;

protected:
	ShaderProgram::Sptr _shader;

};