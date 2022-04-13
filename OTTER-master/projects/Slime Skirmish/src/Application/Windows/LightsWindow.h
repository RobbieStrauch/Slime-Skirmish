#pragma once
#include "../IEditorWindow.h"
#include "../Layers/RenderLayer.h"

/**
 * Handles an editor window for rendering our game object hierarchy
 */
class LightsWindow : public IEditorWindow {
public:
	MAKE_PTRS(LightsWindow);

	LightsWindow();
	virtual ~LightsWindow();

	// Inherited from IEditorWindow

	virtual void Render() override;

protected:
	void _ClearFramebuffer(const Framebuffer::Sptr& buffer, const glm::vec4* colors, int layers);
	void _RenderEffect(const ShaderProgram::Sptr& value);
	float diffuseStrength = 1.0f;
	float specularStrength = 1.0f;
	float ambientStrength = 1.0f;
};