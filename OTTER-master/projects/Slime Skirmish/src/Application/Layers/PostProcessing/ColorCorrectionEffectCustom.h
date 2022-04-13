#pragma once
#include "Application/Layers/PostProcessingLayer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture3D.h"

class ColorCorrectionEffectCustom : public PostProcessingLayer::Effect {
public:
	MAKE_PTRS(ColorCorrectionEffectCustom);
	Texture3D::Sptr Lutcu;

	ColorCorrectionEffectCustom();
	ColorCorrectionEffectCustom(bool defaultLut);
	virtual ~ColorCorrectionEffectCustom();

	virtual void Apply(const Framebuffer::Sptr& gBuffer) override;
	virtual void RenderImGui() override;

	// Inherited from IResource

	ColorCorrectionEffectCustom::Sptr FromJson(const nlohmann::json& data);
	virtual nlohmann::json ToJson() const override;

protected:
	ShaderProgram::Sptr _shader;
	float _strength;
};
