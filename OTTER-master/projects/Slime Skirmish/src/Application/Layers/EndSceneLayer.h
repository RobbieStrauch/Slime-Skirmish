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

using namespace Gameplay;

class EndSceneLayer final : public ApplicationLayer {
public:
	MAKE_PTRS(EndSceneLayer)

	EndSceneLayer();
	virtual ~EndSceneLayer();

	// Inherited from ApplicationLayer
	virtual void OnAppLoad(const nlohmann::json& config) override;
	virtual void OnUpdate() override;

	void DeclareButton(GameObject::Sptr textPanel, glm::vec2 mouse);
	void RestartButton(GameObject::Sptr restartButton, glm::vec2 mouse);
	void ExitButton(GameObject::Sptr exitButton, glm::vec2 mouse);

protected:
	void _CreateScene();
	void _MainLoop();

	bool endReading = false;
	const int buttonDistance = 50;
};