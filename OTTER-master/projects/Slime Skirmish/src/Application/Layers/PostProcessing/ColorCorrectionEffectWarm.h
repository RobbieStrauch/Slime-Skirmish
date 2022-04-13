#pragma once
#include "Application/Layers/PostProcessingLayer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture3D.h"

class ColorCorrectionEffectWarm : public PostProcessingLayer::Effect {
public:
	MAKE_PTRS(ColorCorrectionEffectWarm);
	Texture3D::Sptr Lutw;

	ColorCorrectionEffectWarm();
	ColorCorrectionEffectWarm(bool defaultLut);
	virtual ~ColorCorrectionEffectWarm();

	virtual void Apply(const Framebuffer::Sptr& gBuffer) override;
	virtual void RenderImGui() override;

	// Inherited from IResource

	ColorCorrectionEffectWarm::Sptr FromJson(const nlohmann::json& data);
	virtual nlohmann::json ToJson() const override;

protected:
	ShaderProgram::Sptr _shader;
	float _strength;
};