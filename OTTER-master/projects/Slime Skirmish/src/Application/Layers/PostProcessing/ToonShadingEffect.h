#pragma once
#include "Application/Layers/PostProcessingLayer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture3D.h"

class ToonShadingEffect : public PostProcessingLayer::Effect {
public:
	MAKE_PTRS(ToonShadingEffect);

	ToonShadingEffect();
	virtual ~ToonShadingEffect();

	virtual void Apply(const Framebuffer::Sptr& gBuffer) override;
	virtual void RenderImGui() override;

	// Inherited from IResource

	ToonShadingEffect::Sptr FromJson(const nlohmann::json& data);
	virtual nlohmann::json ToJson() const override;

protected:
	ShaderProgram::Sptr _shader;
	glm::vec4           _OutlineColor;
	float               _depthThreshold;
	float               _normalThreshold;
	float               _depthNormalThreshold;
	float               _depthNormalThresholdScale;
	glm::vec2			_pixelsize;

};
