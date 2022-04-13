#pragma once
#include "Application/Layers/PostProcessingLayer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture3D.h"

class ColorCorrectionEffectCool : public PostProcessingLayer::Effect {
public:
	MAKE_PTRS(ColorCorrectionEffectCool);
	Texture3D::Sptr Lutc;

	ColorCorrectionEffectCool();
	ColorCorrectionEffectCool(bool defaultLut);
	virtual ~ColorCorrectionEffectCool();

	virtual void Apply(const Framebuffer::Sptr& gBuffer) override;
	virtual void RenderImGui() override;

	// Inherited from IResource

	ColorCorrectionEffectCool::Sptr FromJson(const nlohmann::json& data);
	virtual nlohmann::json ToJson() const override;

protected:
	ShaderProgram::Sptr _shader;
	float _strength;
};

