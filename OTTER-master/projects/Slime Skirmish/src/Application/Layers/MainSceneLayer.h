#pragma once
#include "Application/ApplicationLayer.h"
#include "Application/Timing.h"
#include "json.hpp"
#include <queue>

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
#include "Application/Layers/PostProcessing/ColorCorrectionEffectCool.h"
#include "Application/Layers/PostProcessing/ColorCorrectionEffectWarm.h"
#include "Application/Layers/PostProcessing/FilmGrainEffect.h"
#include "Application/Layers/PostProcessing/PixelationEffect.h"
#include "Application/Layers/MenuSceneLayer.h"

using namespace Gameplay;

class MainSceneLayer final : public ApplicationLayer {
public:
	MAKE_PTRS(MainSceneLayer)

		MainSceneLayer();
	virtual ~MainSceneLayer();

	// Inherited from ApplicationLayer
	virtual void OnAppLoad(const nlohmann::json& config) override;
	virtual void OnUpdate() override;

protected:
	template<typename T>
	T LERP(const T& p0, const T& p1, float t)
	{
		return (1.0f - t) * p0 + t * p1;
	}
	void DeclareButton(GameObject::Sptr textPanel, glm::vec2 mouse);
	void ResumeButton(GameObject::Sptr pausePanel, GameObject::Sptr resumeButton, glm::vec2 mouse);
	void ExitButton(GameObject::Sptr player, GameObject::Sptr pausePanel, GameObject::Sptr exitButton, glm::vec2 mouse);
	void TutorialButtons(GameObject::Sptr tutorial, GameObject::Sptr tutorialPanel, GameObject::Sptr nextButton, GameObject::Sptr skipButton, glm::vec2 mouse);
	void CreateEnemies(Scene::Sptr scene, std::vector<GameObject::Sptr> spawns, GameObject::Sptr player, std::vector<GameObject::Sptr> walls);
	void CreateSuperEnemies(Scene::Sptr scene, std::vector<GameObject::Sptr> spawns, GameObject::Sptr player, std::vector<GameObject::Sptr> walls);
	void TopDownCamera(GameObject* camera, GameObject::Sptr player);

	void _CreateScene();
	void _MainLoop();

	AudioEngine* engine = AudioEngine::Instance();
	AudioSource theme;
	float cameraHeight = 15.0f;
	float cameraDistance = 8.0f;
	bool cameraShift = false;
	float cameraLerp = 0.1f;
	float startSize = 0.5f;
	const int buttonDistance = 50;
	MeshResource::Sptr enemyMesh;
	Material::Sptr enemyMaterial;
	MeshResource::Sptr superEnemyMesh;
	Material::Sptr superEnemyMaterial;
	Material::Sptr attackMaterial;
	MeshResource::Sptr backstandStart;
	int enemyAmount = 0;
	int enemyCount = 5;
	int superEnemyAmount = 0;
	int superEnemyCount = 5;
	int wallAmount = 8;
	int superWallAmount = 5;
	bool deleteWall = false;
	Animations enemyWalk;
	Animations enemyAttack;
	Animations superEnemyWalk;
	Animations superEnemyAttack;
	Animations slimeMove;
	Animations slimeEat;
	Animations slimeIdle;
	Animations bannerWind;
	bool isPaused = true;
	Texture2D::Sptr step0;
	Texture2D::Sptr step1;
	Texture2D::Sptr step2;
	std::queue<Texture2D::Sptr> steps;
	std::queue<Texture2D::Sptr> stepsTemp;
	bool tutorialBool[3] = { true, false, false };
	bool tutorialStart = true;
	bool isTutorial = true;
	float tutorialTime;
	float soundEffectVolume;
	float arenaLerp = 0.0f;
	float shakeLerp = 0.0f;
	bool destroyArena = true;
	bool test = false;
};