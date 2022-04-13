#pragma once
#include "Application/ApplicationLayer.h"
#include "Application/Timing.h"
#include "json.hpp"

// Graphics
#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/Buffers/VertexBuffer.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture2D.h"
#include "Graphics/Textures/TextureCube.h"
#include "Graphics/VertexTypes.h"
#include "Graphics/Font.h"
#include "Graphics/GuiBatcher.h"
#include "Graphics/Framebuffer.h"

// Utilities
#include "Utils/MeshBuilder.h"
#include "Utils/MeshFactory.h"
#include "Utils/ObjLoader.h"
#include "Utils/ImGuiHelper.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/FileHelpers.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/StringUtils.h"
#include "Utils/GlmDefines.h"

// Gameplay
#include "Gameplay/Material.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Gameplay/Animations.h"

// Audio
#include "Gameplay/Audio/AudioEngine.h"
#include "Gameplay/Audio/AudioSource.h"
#include "Gameplay/Audio/AudioListener.h"

// Components
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/Components/Camera.h"
#include "Gameplay/Components/JumpBehaviour.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Components/MaterialSwapBehaviour.h"
#include "Gameplay/Components/SimpleCameraControl.h"
#include "Gameplay/Components/ParticleSystem.h"
#include "Gameplay/Components/Light.h"
#include "Gameplay/Components/ShadowCamera.h"

// Custom Components
#include "Gameplay/Components/Custom/RotatingBehaviour.h"
#include "Gameplay/Components/Custom/TriggerVolumeEnterBehaviour.h"
#include "Gameplay/Components/Custom/AbilityComponent.h"
#include "Gameplay/Components/Custom/MovementComponent.h"
#include "Gameplay/Components/Custom/EnemyComponent.h"
#include "Gameplay/Components/Custom/ArrivalBehaviour.h"
#include "Gameplay/Components/Custom/MorphTarget.h"
#include "Gameplay/Components/Custom/TopDownCamera.h"
#include "Gameplay/Components/Custom/ScoreKeep.h"
#include "Gameplay/Components/Custom/PauseComponent.h"
#include "Gameplay/Components/Custom/FMODStudio.h"
#include "Gameplay/Components/Custom/SuperEnemyComponent.h"

// Physics
#include "Gameplay/Physics/RigidBody.h"
#include "Gameplay/Physics/Colliders/BoxCollider.h"
#include "Gameplay/Physics/Colliders/PlaneCollider.h"
#include "Gameplay/Physics/Colliders/SphereCollider.h"
#include "Gameplay/Physics/Colliders/ConvexMeshCollider.h"
#include "Gameplay/Physics/Colliders/CylinderCollider.h"
#include "Gameplay/Physics/TriggerVolume.h"
#include "Graphics/DebugDraw.h"

// GUI
#include "Gameplay/Components/GUI/RectTransform.h"
#include "Gameplay/Components/GUI/GuiPanel.h"
#include "Gameplay/Components/GUI/GuiText.h"
#include "Gameplay/InputEngine.h"

#include "Application/Application.h"
#include "Gameplay/Components/ParticleSystem.h"
#include "Graphics/Textures/Texture3D.h"
#include "Graphics/Textures/Texture1D.h"

// Layers
#include "Application/Layers/PostProcessingLayer.h"
#include "Application/Layers/PostProcessing/ColorCorrectionEffectCool.h"
#include "Application/Layers/PostProcessing/ColorCorrectionEffectWarm.h"
#include "Application/Layers/PostProcessing/FilmGrainEffect.h"
#include "Application/Layers/PostProcessing/PixelationEffect.h"

using namespace Gameplay;

class MenuSceneLayer final : public ApplicationLayer {
public:
	MAKE_PTRS(MenuSceneLayer)

	MenuSceneLayer();
	virtual ~MenuSceneLayer();

	// Inherited from ApplicationLayer
	virtual void OnAppLoad(const nlohmann::json& config) override;
	virtual void OnUpdate() override;

	bool textureToggle = true;
	bool shadowMapping = true;
	bool lightingToggle = true;
	float musicVolume = 5.0f;
	float effectVolume = 5.0f;
	float dialogueVolume = 5.0f;

protected:
	template<typename T>
	T LERP(const T& p0, const T& p1, float t)
	{
		return (1.0f - t) * p0 + t * p1;
	}
	void DeclareButton(GameObject::Sptr textPanel, glm::vec2 mouse);
	void DeclareButton(GameObject::Sptr textPanel, glm::vec2 mouse, bool shorter);
	void StartButton(GameObject::Sptr startButton, glm::vec2 mouse);
	void OptionsButton(GameObject::Sptr optionsButton, glm::vec2 mouse);
	void ExitButton(GameObject::Sptr exitButton, glm::vec2 mouse);
	void BackButton(GameObject::Sptr backButton, glm::vec2 mouse);
	void SoundButton(GameObject::Sptr soundButton, glm::vec2 mouse);
	void VolumeButtons(GameObject::Sptr volumeText, GameObject::Sptr volumeMinus, GameObject::Sptr volumePlus, glm::vec2 mouse);
	void EffectVolumeButtons(GameObject::Sptr effectVolumeText, GameObject::Sptr effectVolumeMinus, GameObject::Sptr effectVolumePlus, glm::vec2 mouse);
	void DialogueVolumeButtons(GameObject::Sptr dialogueVolumeText, GameObject::Sptr dialogueVolumeMinus, GameObject::Sptr dialogueVolumePlus, glm::vec2 mouse);
	void ControlButton(GameObject::Sptr controlButton, glm::vec2 mouse);
	void GraphicButton(GameObject::Sptr graphicButton, glm::vec2 mouse);
	void WarmColorCorrectionButton(GameObject::Sptr warmColorCorrectButton, glm::vec2 mouse);
	void CoolColorCorrectionButton(GameObject::Sptr coolColorCorrectButton, glm::vec2 mouse);
	void PixelButton(GameObject::Sptr pixelButton, glm::vec2 mouse);
	void FilmGrainButton(GameObject::Sptr filmGrainButton, glm::vec2 mouse);
	void TextureButton(GameObject::Sptr textureButton, glm::vec2 mouse);
	void ShadowMappingButton(GameObject::Sptr shadowMapButton, GameObject::Sptr shadowMap, glm::vec2 mouse);
	void LightingButton(GameObject::Sptr lightingButton, GameObject::Sptr shadowMap, glm::vec2 mouse);

	void _CreateScene();
	void _MainLoop();

	const int buttonDistance = 50;
	bool options = false;
	bool controls = false;
	bool sounds = false;
	bool graphics = false;
	bool pressed = false;
	Animations bannerWind;
	bool warmColorCorrect = false;
	bool coolColorCorrect = false;
	bool pixelEffect = false;
	bool filmGrainEffect = false;
};