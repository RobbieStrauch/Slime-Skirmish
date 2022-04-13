// GLM math library
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <chrono>
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/common.hpp> // for fmod (floating modulus)
#include "GLM/gtx/matrix_decompose.hpp"
#include <GLM/gtc/random.hpp>

#include <filesystem>
#include <random>
#include <memory>
#include <fstream>

#include "Logging.h"
#include "imgui.h"

#include "MainSceneLayer.h"

using namespace Gameplay;
using namespace Gameplay::Physics;
using namespace std::chrono;

MainSceneLayer::MainSceneLayer() : ApplicationLayer()
{
	Name = "Main Scene";
	Overrides = AppLayerFunctions::OnAppLoad | AppLayerFunctions::OnUpdate;
}

MainSceneLayer::~MainSceneLayer() = default;

void MainSceneLayer::OnAppLoad(const nlohmann::json& config)
{
	_CreateScene();
}

void MainSceneLayer::OnUpdate()
{
	if (Application::Get().CurrentScene()->FindObjectByName("Player") != nullptr)
	{
		_MainLoop();
	}
	else
	{
		theme.SetPause(true);
	}
}

void MainSceneLayer::DeclareButton(GameObject::Sptr textPanel, glm::vec2 mouse)
{
	double distance = glm::distance(mouse, textPanel->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance)
	{
		textPanel->Get<GuiText>()->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		textPanel->Get<GuiText>()->SetColor(glm::vec4(1.0f));
	}
}

void MainSceneLayer::ResumeButton(GameObject::Sptr pausePanel, GameObject::Sptr resumeButton, glm::vec2 mouse)
{
	DeclareButton(resumeButton, mouse);
	double distance = glm::distance(mouse, resumeButton->Get<RectTransform>()->GetPosition());

	if ((distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed))
	{
		pausePanel->Get<PauseComponent>()->isPaused = false;
		pausePanel->Get<PauseComponent>()->pressed = true;
	}
}

void MainSceneLayer::ExitButton(GameObject::Sptr player, GameObject::Sptr pausePanel, GameObject::Sptr exitButton, glm::vec2 mouse)
{
	DeclareButton(exitButton, mouse);
	double distance = glm::distance(mouse, exitButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed && std::filesystem::exists("scenes/menu-screen.json") /*&& isPaused*/)
	{
		pausePanel->Get<PauseComponent>()->isPaused = false;
		player->Score.y = 0;
		player->Score.x = 0;
		player->Score.z = 0;
		player->Get<ScoreKeep>()->bestSize = 5;
		player->Get<ScoreKeep>()->nextSpawn = 0.0f;
		player->Get<ScoreKeep>()->enemyCounter = 0;

		Application::Get().LoadScene("scenes/menu-screen.json");
	}
}

void MainSceneLayer::TutorialButtons(GameObject::Sptr tutorial, GameObject::Sptr tutorialPanel, GameObject::Sptr nextButton, GameObject::Sptr skipButton, glm::vec2 mouse)
{
	Application& app = Application::Get();
	Timing& time = Timing::Current();

	// Start Timing
	auto start = high_resolution_clock::now();

	// Function/Loop to measure time complexity of goes here
	auto result = 0;
	auto n = 1000;


	if (tutorial != nullptr)
	{
		tutorialPanel->Get<RectTransform>()->SetPosition({ app.GetWindowSize().x / 2, app.GetWindowSize().y / 2 });
		nextButton->Get<RectTransform>()->SetPosition({ app.GetWindowSize().x / 2, app.GetWindowSize().y / 2 + 150 });
		skipButton->Get<RectTransform>()->SetPosition({ app.GetWindowSize().x / 2 + 300,  app.GetWindowSize().y / 2 - 150 });

		time.SetTimeScale(0);

		if (tutorialBool[0] || tutorialBool[1] || tutorialBool[2])
		{
			isTutorial = true;
			//DeclareButton(backButton, mouse);
			DeclareButton(nextButton, mouse);
			DeclareButton(skipButton, mouse);

			//double distanceBack = glm::distance(mouse, backButton->Get<RectTransform>()->GetPosition());
			double distanceNext = glm::distance(mouse, nextButton->Get<RectTransform>()->GetPosition());
			double distanceSkip = glm::distance(mouse, skipButton->Get<RectTransform>()->GetPosition());

			if (tutorialBool[0] && tutorialStart)
			{
				theme.SetPause(true);
				theme.SetSound("Tutorial 1");
				theme.Play();
				theme.SetVolume(app.GetLayer<MenuSceneLayer>()->dialogueVolume);
				theme.SetLoop(false);
				theme.SetPause(false);

				tutorialStart = false;
			}
			if (tutorialBool[0] && distanceNext < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
			{
				theme.SetPause(true);
				theme.SetSound("Tutorial 2");
				theme.Play();
				theme.SetVolume(app.GetLayer<MenuSceneLayer>()->dialogueVolume);
				theme.SetLoop(false);
				theme.SetPause(false);

				stepsTemp.push(steps.front());
				steps.pop();
				tutorialPanel->Get<GuiPanel>()->SetTexture(steps.front());
				tutorialBool[0] = false;
				tutorialBool[1] = true;
				glfwWaitEventsTimeout(1.0f);
			}
			if (tutorialBool[1] && distanceNext < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
			{
				theme.SetPause(true);
				theme.SetSound("Tutorial 3");
				theme.Play();
				theme.SetVolume(app.GetLayer<MenuSceneLayer>()->dialogueVolume);
				theme.SetLoop(false);
				theme.SetPause(false);

				stepsTemp.push(steps.front());
				steps.pop();
				tutorialPanel->Get<GuiPanel>()->SetTexture(steps.front());
				tutorialBool[1] = false;
				tutorialBool[2] = true;
				glfwWaitEventsTimeout(1.0f);
			}
			if (tutorialBool[2])
			{
				nextButton->Get<GuiText>()->SetText("Finish");
			}
			else
			{
				nextButton->Get<GuiText>()->SetText("Next");
			}
			//if (tutorialBool[1] && distanceBack < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
			//{
			//	tutorialPanel->Get<GuiPanel>()->SetTexture(step0);
			//	tutorialBool[1] = false;
			//	tutorialBool[0] = true;
			//	glfwWaitEventsTimeout(1.0f);
			//}
			if (tutorialBool[2] && distanceNext < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
			{
				stepsTemp.push(steps.front());
				steps.pop();
				tutorialBool[2] = false;
				glfwWaitEventsTimeout(1.0f);
			}
			//if (tutorialBool[2] && distanceBack < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
			//{
			//	tutorialPanel->Get<GuiPanel>()->SetTexture(step1);
			//	tutorialBool[2] = false;
			//	tutorialBool[1] = true;
			//	glfwWaitEventsTimeout(1.0f);
			//}
			if (distanceSkip < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
			{
				tutorialBool[0] = false;
				tutorialBool[1] = false;
				tutorialBool[2] = false;
				tutorialStart = false;
				while (!steps.empty())
				{
					steps.pop();
				}
				glfwWaitEventsTimeout(1.0f);
			}
		}
		if (!tutorialBool[0] && !tutorialBool[1] && !tutorialBool[2])
		{
			theme.SetPause(true);
			theme.SetSound("Theme 1");
			theme.Play();
			theme.SetVolume(app.GetLayer<MenuSceneLayer>()->musicVolume);
			theme.SetLoop(true);
			theme.SetPause(false);

			isTutorial = false;
			app.CurrentScene()->RemoveGameObject(tutorial->SelfRef());
			app.CurrentScene()->RemoveGameObject(tutorialPanel->SelfRef());
			//app.CurrentScene()->RemoveGameObject(backButton->SelfRef());
			app.CurrentScene()->RemoveGameObject(nextButton->SelfRef());
			app.CurrentScene()->RemoveGameObject(skipButton->SelfRef());
			time.SetTimeScale(1);
		}
	}

	// Stop Timing
	auto stop = high_resolution_clock::now();

	// Unit of Time
	auto duration = duration_cast<microseconds>(stop - start);

	// Output Duration
	//std::cout << "n = " << n << "\nTime of Loop: " << duration.count() << " microseconds" << std::endl;
}

void MainSceneLayer::CreateEnemies(Scene::Sptr scene, std::vector<GameObject::Sptr> spawns, GameObject::Sptr player, std::vector<GameObject::Sptr> walls)
{
	enemyAmount += enemyCount;

	std::vector<GameObject::Sptr> enemy;
	enemy.resize(enemyAmount);

	std::vector<int> spawnCount;

	for (size_t j = 0; j < 4; j++)
	{	
		if (!spawns[j]->Get<TriggerVolumeEnterBehaviour>()->GetSpawnTrigger()) continue;
		spawnCount.push_back(j);
	}

	for (size_t i = enemyAmount - enemyCount; i < enemyAmount; i++)
	{
		enemy[i] = scene->CreateGameObject("Enemy" + std::to_string(i));

		int randSize = rand() % 3 + 1;

		int randomSpawn = spawnCount[rand() % spawnCount.size()];

		enemy[i]->SetPostion(glm::vec3(spawns[randomSpawn]->GetPosition().x, spawns[randomSpawn]->GetPosition().y, -1.75f));

		enemy[i]->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
		enemy[i]->SetScale(glm::vec3(0.35f + (randSize / 10.0f)));
		enemy[i]->SetSize(randSize);

		enemy[i]->Add<AbilityComponent>();
		
		EnemyComponent::Sptr enem = enemy[i]->Add<EnemyComponent>();
		enem->SetPlayer(player);

		ArrivalBehaviour::Sptr arrival = enemy[i]->Add<ArrivalBehaviour>();
		arrival->SetPlayer(player);
		arrival->SetWalls(walls);

		RenderComponent::Sptr renderer = enemy[i]->Add<RenderComponent>();
		renderer->SetMesh(enemyMesh);
		renderer->SetMaterial(enemyMaterial);

		TriggerVolume::Sptr trigger = enemy[i]->Add<TriggerVolume>();
		CylinderCollider::Sptr cylinder = CylinderCollider::Create(glm::vec3(2.0f, 2.0f, 1.0f));
		cylinder->SetPosition(glm::vec3(0.0f, 1.0f, 2.0f));
		cylinder->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
		trigger->AddCollider(cylinder);

		TriggerVolumeEnterBehaviour::Sptr test = enemy[i]->Add<TriggerVolumeEnterBehaviour>();
		test->SetPlayerTrigger(false);

		MorphTarget::Sptr anim = enemy[i]->Add<MorphTarget>();
		anim->SetType(AnimationType::Idle);
	}
}

void MainSceneLayer::CreateSuperEnemies(Scene::Sptr scene, std::vector<GameObject::Sptr> spawns, GameObject::Sptr player, std::vector<GameObject::Sptr> walls)
{
	superEnemyAmount += superEnemyCount;

	std::vector<GameObject::Sptr> superEnemy;
	superEnemy.resize(superEnemyAmount);

	for (size_t i = superEnemyAmount - superEnemyCount; i < superEnemyAmount; i++)
	{
		superEnemy[i] = scene->CreateGameObject("Super Enemy" + std::to_string(i));

		int randSize = rand() % 6 + 4;
		int randomSpawn = rand() % spawns.size();

		superEnemy[i]->SetPostion(glm::vec3(spawns[randomSpawn]->GetPosition().x, spawns[randomSpawn]->GetPosition().y, -1.75f));

		superEnemy[i]->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
		superEnemy[i]->SetScale(glm::vec3(0.35f + (randSize / 20.0f)));
		superEnemy[i]->SetSize(randSize);

		superEnemy[i]->Add<AbilityComponent>();

		EnemyComponent::Sptr enem = superEnemy[i]->Add<EnemyComponent>();
		enem->SetPlayer(player);

		ArrivalBehaviour::Sptr arrival = superEnemy[i]->Add<ArrivalBehaviour>();
		arrival->SetPlayer(player);
		arrival->SetWalls(walls);

		RenderComponent::Sptr renderer = superEnemy[i]->Add<RenderComponent>();
		renderer->SetMesh(superEnemyMesh);
		renderer->SetMaterial(superEnemyMaterial);

		TriggerVolume::Sptr trigger = superEnemy[i]->Add<TriggerVolume>();
		CylinderCollider::Sptr cylinder = CylinderCollider::Create(glm::vec3(4.5f, 4.5f, 1.0f));
		cylinder->SetPosition(glm::vec3(0.0f, 1.0f, -4.5f));
		cylinder->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
		trigger->AddCollider(cylinder);

		TriggerVolumeEnterBehaviour::Sptr test = superEnemy[i]->Add<TriggerVolumeEnterBehaviour>();
		test->SetPlayerTrigger(false);

		MorphTarget::Sptr anim = superEnemy[i]->Add<MorphTarget>();
		anim->SetType(AnimationType::Idle);
	}
}

void MainSceneLayer::TopDownCamera(GameObject* camera, GameObject::Sptr player)
{
	glm::vec3 cameraPosition = (glm::vec3(0.0f, -cameraDistance, 0.0f)) + (glm::vec3(0.0f, 0.0f, cameraHeight));
	camera->SetPostion(player->GetPosition() + cameraPosition);
	camera->LookAt(player->GetPosition());
}

void MainSceneLayer::_CreateScene()
{
	Application& app = Application::Get();
	bool loadScene = false;

	if (loadScene && std::filesystem::exists("scene.json")) 
	{
		app.LoadScene("scene.json");
	} 
	else
	{
		// Create shaders
		ShaderProgram::Sptr deferredForward = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/deferred_forward.glsl" }
		});
		deferredForward->SetDebugName("Deferred - GBuffer Generation");

		// Load meshes
		MeshResource::Sptr goblinMesh = ResourceManager::CreateAsset<MeshResource>("models/Goblin.obj"); enemyMesh = goblinMesh;
		MeshResource::Sptr slimeMesh = ResourceManager::CreateAsset<MeshResource>("models/Slime2.obj");
		MeshResource::Sptr orcMesh = ResourceManager::CreateAsset<MeshResource>("models/Orc.obj"); superEnemyMesh = goblinMesh;

		MeshResource::Sptr backstandMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Backstand.obj"); backstandStart = backstandMesh;
		MeshResource::Sptr flagMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Flag.obj");
		MeshResource::Sptr gatesMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Gates.obj");
		MeshResource::Sptr groundMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Ground.obj");
		MeshResource::Sptr polesMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Poles.obj");
		MeshResource::Sptr polesMesh2 = ResourceManager::CreateAsset<MeshResource>("models/arena/Poles2.obj");
		MeshResource::Sptr standsMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Stands.obj");
		MeshResource::Sptr standsMesh2 = ResourceManager::CreateAsset<MeshResource>("models/arena/Stands2.obj");
		MeshResource::Sptr wallsMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Walls.obj");

		MeshResource::Sptr bannerMesh = ResourceManager::CreateAsset<MeshResource>("models/animations/Banner/Banner_1.obj");

		MeshResource::Sptr campMesh = ResourceManager::CreateAsset<MeshResource>("models/mountain/Camp.obj");
		MeshResource::Sptr doubleRockMesh = ResourceManager::CreateAsset<MeshResource>("models/mountain/DoubleRock.obj");
		MeshResource::Sptr fenceMesh = ResourceManager::CreateAsset<MeshResource>("models/mountain/Fence.obj");
		MeshResource::Sptr fenceMesh2 = ResourceManager::CreateAsset<MeshResource>("models/mountain/Fence2.obj");
		MeshResource::Sptr longRockMesh = ResourceManager::CreateAsset<MeshResource>("models/mountain/LongRock.obj");
		MeshResource::Sptr mountainMesh = ResourceManager::CreateAsset<MeshResource>("models/mountain/Mountain.obj");
		MeshResource::Sptr orcSpawnMesh = ResourceManager::CreateAsset<MeshResource>("models/mountain/OrcSpawn.obj");
		MeshResource::Sptr hayMesh = ResourceManager::CreateAsset<MeshResource>("models/mountain/Hay.obj");
		MeshResource::Sptr envMountainMesh = ResourceManager::CreateAsset<MeshResource>("models/mountain/EnvMountain.obj");

		// Load textures
		Texture2D::Sptr greenTexture = ResourceManager::CreateAsset<Texture2D>("textures/green.png");
		Texture2D::Sptr greyTexture = ResourceManager::CreateAsset<Texture2D>("textures/grey.png");
		Texture2D::Sptr redTexture = ResourceManager::CreateAsset<Texture2D>("textures/red.png");
		Texture2D::Sptr slime_tex = ResourceManager::CreateAsset<Texture2D>("textures/SlimeTex.png");

		Texture2D::Sptr tutorial0 = ResourceManager::CreateAsset<Texture2D>("images/tutorial/Tutorial0.png");
		Texture2D::Sptr tutorial1 = ResourceManager::CreateAsset<Texture2D>("images/tutorial/Tutorial1.png");
		Texture2D::Sptr tutorial2 = ResourceManager::CreateAsset<Texture2D>("images/tutorial/Tutorial2.png");
		steps.push(tutorial0);
		steps.push(tutorial1);
		steps.push(tutorial2);

		Texture2D::Sptr sand = ResourceManager::CreateAsset<Texture2D>("textures/arena/Sand.png");
		Texture2D::Sptr pole_tex = ResourceManager::CreateAsset<Texture2D>("textures/arena/Pole_Tex.png");
		Texture2D::Sptr gate_tex = ResourceManager::CreateAsset<Texture2D>("textures/arena/Gate_Tex.png");
		Texture2D::Sptr flag_tex = ResourceManager::CreateAsset<Texture2D>("textures/arena/Flag Tex.png");
		Texture2D::Sptr backstand_tex = ResourceManager::CreateAsset<Texture2D>("textures/arena/BackStand_Tex.png");
		Texture2D::Sptr sandstone_tex = ResourceManager::CreateAsset<Texture2D>("textures/arena/SandStone_Tex.png");
		Texture2D::Sptr wood_tex = ResourceManager::CreateAsset<Texture2D>("textures/arena/Wood_Tex.png");

		Texture2D::Sptr banner_tex = ResourceManager::CreateAsset<Texture2D>("textures/BannerTexture.png");

		Texture2D::Sptr fence_tex = ResourceManager::CreateAsset<Texture2D>("textures/mountain/Fence.png");
		Texture2D::Sptr mountain_tex = ResourceManager::CreateAsset<Texture2D>("textures/mountain/MountainTex.png");
		Texture2D::Sptr orc_spawnTex = ResourceManager::CreateAsset<Texture2D>("textures/mountain/Orc_Spawn.png");
		Texture2D::Sptr rock_tex = ResourceManager::CreateAsset<Texture2D>("textures/mountain/RockTex.png");
		Texture2D::Sptr fire_tex = ResourceManager::CreateAsset<Texture2D>("textures/mountain/FireTex.png");
		Texture2D::Sptr hay_tex = ResourceManager::CreateAsset<Texture2D>("textures/mountain/HayTex.png");
		Texture2DArray::Sptr particleTex = ResourceManager::CreateAsset<Texture2DArray>("textures/particles.png", 2, 2);

#pragma region Basic Texture Creation
		Texture2DDescription singlePixelDescriptor;
		singlePixelDescriptor.Width = singlePixelDescriptor.Height = 1;
		singlePixelDescriptor.Format = InternalFormat::RGB8;

		float normalMapDefaultData[3] = { 0.5f, 0.5f, 1.0f };
		Texture2D::Sptr normalMapDefault = ResourceManager::CreateAsset<Texture2D>(singlePixelDescriptor);
		normalMapDefault->LoadData(1, 1, PixelFormat::RGB, PixelType::Float, normalMapDefaultData);
#pragma endregion 

		// Here we'll load in the cubemap, as well as a special shader to handle drawing the skybox
		TextureCube::Sptr testCubemap = ResourceManager::CreateAsset<TextureCube>("cubemaps/ocean/ocean.jpg");
		ShaderProgram::Sptr      skyboxShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/skybox_vert.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/skybox_frag.glsl" }});

		// Create an empty scene
		Scene::Sptr scene = std::make_shared<Scene>();

		// Setting up our enviroment map
		scene->SetSkyboxTexture(testCubemap);
		scene->SetSkyboxShader(skyboxShader);
		scene->SetSkyboxRotation(glm::rotate(MAT4_IDENTITY, glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)));

		// Loading in a color lookup table
		Texture3D::Sptr lut = ResourceManager::CreateAsset<Texture3D>("luts/cool.CUBE");

		// Configure the color correction LUT
		scene->SetColorLUT(lut);

		// Create our materials
		Material::Sptr greenMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			greenMaterial->Name = "Green";
			greenMaterial->Set("TextureToggle", true);
			greenMaterial->Set("u_Material.AlbedoMap", greenTexture);
			greenMaterial->Set("u_Material.NormalMap", normalMapDefault);
			greenMaterial->Set("u_Material.EmissiveMap", greenTexture);
		} enemyMaterial = greenMaterial;
		Material::Sptr greyMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			greyMaterial->Name = "Grey";
			greyMaterial->Set("TextureToggle", true);
			greyMaterial->Set("u_Material.AlbedoMap", greyTexture);
			greyMaterial->Set("u_Material.NormalMap", normalMapDefault);
			greyMaterial->Set("u_Material.EmissiveMap", greyTexture);
		} superEnemyMaterial = greyMaterial;
		Material::Sptr redMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			redMaterial->Name = "Red";
			redMaterial->Set("TextureToggle", true);
			redMaterial->Set("u_Material.AlbedoMap", redTexture);
			redMaterial->Set("u_Material.NormalMap", normalMapDefault);
			redMaterial->Set("u_Material.EmissiveMap", redTexture);
		} attackMaterial = redMaterial;
		Material::Sptr slimeMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			slimeMaterial->Name = "Slime Material";
			slimeMaterial->Set("TextureToggle", true);
			slimeMaterial->Set("u_Material.AlbedoMap", slime_tex);
			slimeMaterial->Set("u_Material.NormalMap", normalMapDefault);
			slimeMaterial->Set("u_Material.EmissiveMap", slime_tex);
			slimeMaterial->Set("u_Material.MetallicShininessMap", slime_tex);
		}

		Material::Sptr sandMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			sandMaterial->Name = "Sand";
			sandMaterial->Set("TextureToggle", true);
			sandMaterial->Set("u_Material.AlbedoMap", sand);
			sandMaterial->Set("u_Material.NormalMap", normalMapDefault);
			sandMaterial->Set("u_Material.EmissiveMap", sand);
		}
		Material::Sptr pole_texMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			pole_texMaterial->Name = "Pole";
			pole_texMaterial->Set("TextureToggle", true);
			pole_texMaterial->Set("u_Material.AlbedoMap", pole_tex);
			pole_texMaterial->Set("u_Material.NormalMap", normalMapDefault);
			pole_texMaterial->Set("u_Material.EmissiveMap", pole_tex);
		}
		Material::Sptr gate_texMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			gate_texMaterial->Name = "Gate";
			gate_texMaterial->Set("TextureToggle", true);
			gate_texMaterial->Set("u_Material.AlbedoMap", gate_tex);
			gate_texMaterial->Set("u_Material.NormalMap", normalMapDefault);
			gate_texMaterial->Set("u_Material.EmissiveMap", gate_tex);
		}
		Material::Sptr flag_texMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			flag_texMaterial->Name = "Flag";
			flag_texMaterial->Set("TextureToggle", true);
			flag_texMaterial->Set("u_Material.AlbedoMap", flag_tex);
			flag_texMaterial->Set("u_Material.NormalMap", normalMapDefault);
			flag_texMaterial->Set("u_Material.EmissiveMap", flag_tex);
		}
		Material::Sptr backstand_texMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			backstand_texMaterial->Name = "Back Stand";
			backstand_texMaterial->Set("TextureToggle", true);
			backstand_texMaterial->Set("u_Material.AlbedoMap", backstand_tex);
			backstand_texMaterial->Set("u_Material.NormalMap", normalMapDefault);
			backstand_texMaterial->Set("u_Material.EmissiveMap", backstand_tex);
		}
		Material::Sptr sandstone_texMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			sandstone_texMaterial->Name = "Sand Stone";
			sandstone_texMaterial->Set("TextureToggle", true);
			sandstone_texMaterial->Set("u_Material.AlbedoMap", sandstone_tex);
			sandstone_texMaterial->Set("u_Material.NormalMap", normalMapDefault);
			sandstone_texMaterial->Set("u_Material.EmissiveMap", sandstone_tex);
		}
		Material::Sptr wood_texMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			wood_texMaterial->Name = "Wood";
			wood_texMaterial->Set("TextureToggle", true);
			wood_texMaterial->Set("u_Material.AlbedoMap", wood_tex);
			wood_texMaterial->Set("u_Material.NormalMap", normalMapDefault);
			wood_texMaterial->Set("u_Material.EmissiveMap", wood_tex);
		}
		Material::Sptr banner_texMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			banner_texMaterial->Name = "Banner Material";
			banner_texMaterial->Set("TextureToggle", true);
			banner_texMaterial->Set("u_Material.AlbedoMap", banner_tex);
			banner_texMaterial->Set("u_Material.NormalMap", normalMapDefault);
			banner_texMaterial->Set("u_Material.EmissiveMap", banner_tex);
		}

		Material::Sptr fence_texMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			fence_texMaterial->Name = "Fence Material";
			fence_texMaterial->Set("TextureToggle", true);
			fence_texMaterial->Set("u_Material.AlbedoMap", fence_tex);
			fence_texMaterial->Set("u_Material.NormalMap", normalMapDefault);
			fence_texMaterial->Set("u_Material.EmissiveMap", fence_tex);
		}
		Material::Sptr mountain_texMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			mountain_texMaterial->Name = "Mountain Material";
			mountain_texMaterial->Set("TextureToggle", true);
			mountain_texMaterial->Set("u_Material.AlbedoMap", mountain_tex);
			mountain_texMaterial->Set("u_Material.NormalMap", normalMapDefault);
			mountain_texMaterial->Set("u_Material.EmissiveMap", mountain_tex);
		}
		Material::Sptr orc_spawnTexMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			orc_spawnTexMaterial->Name = "Orc Spawn Material";
			orc_spawnTexMaterial->Set("TextureToggle", true);
			orc_spawnTexMaterial->Set("u_Material.AlbedoMap", orc_spawnTex);
			orc_spawnTexMaterial->Set("u_Material.NormalMap", normalMapDefault);
			orc_spawnTexMaterial->Set("u_Material.EmissiveMap", orc_spawnTex);
		}
		Material::Sptr rock_texMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			rock_texMaterial->Name = "Rock Material";
			rock_texMaterial->Set("TextureToggle", true);
			rock_texMaterial->Set("u_Material.AlbedoMap", rock_tex);
			rock_texMaterial->Set("u_Material.NormalMap", normalMapDefault);
			rock_texMaterial->Set("u_Material.EmissiveMap", rock_tex);
		}
		Material::Sptr fire_texMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			fire_texMaterial->Name = "Fire Material";
			fire_texMaterial->Set("TextureToggle", true);
			fire_texMaterial->Set("u_Material.AlbedoMap", fire_tex);
			fire_texMaterial->Set("u_Material.NormalMap", normalMapDefault);
			fire_texMaterial->Set("u_Material.EmissiveMap", fire_tex);
		}
		Material::Sptr hay_texMaterial = ResourceManager::CreateAsset<Material>(deferredForward);
		{
			hay_texMaterial->Name = "Hay Material";
			hay_texMaterial->Set("TextureToggle", true);
			hay_texMaterial->Set("u_Material.AlbedoMap", hay_tex);
			hay_texMaterial->Set("u_Material.NormalMap", normalMapDefault);
			hay_texMaterial->Set("u_Material.EmissiveMap", hay_tex);
		}

		// Create camera
		GameObject::Sptr camera = scene->MainCamera->GetGameObject()->SelfRef();
		{
			camera->SetPostion(glm::vec3(20.0f, -2.0f, 25.0f));
			camera->LookAt(glm::vec3(20.0f, 15.0f, 0.0f));
			camera->Add<SimpleCameraControl>();
		}

		// Create animation frames
		enemyWalk.SetAnimation("GoblinWalk/Walk_", 49);
		enemyWalk.SetTime(0.07f);

		enemyAttack.SetAnimation("GoblinAttack/Attack_", 18);
		enemyAttack.SetTime(0.08f);

		superEnemyWalk.SetAnimation("OrcWalk/Walk_", 48);
		superEnemyWalk.SetTime(0.08f);

		superEnemyAttack.SetAnimation("OrcAttack/Attack_", 36);
		superEnemyAttack.SetTime(0.09f);

		slimeMove.SetAnimation("SlimeMove/Move_", 6);
		slimeMove.SetTime(0.1f);

		slimeIdle.SetAnimation("SlimeIdle/Idle_", 2);
		slimeIdle.SetTime(0.09f);

		slimeEat.SetAnimation("SlimeEat/Eat_", 5);
		slimeEat.SetTime(0.09f);

		bannerWind.SetAnimation("Banner/Banner_", 125);
		bannerWind.SetTime(0.1f);

		// Create game objects
		GameObject::Sptr player = scene->CreateGameObject("Player");
		{
			player->SetPostion(glm::vec3(20.0f, 15.0f, 0.0f));
			player->SetRotation(glm::vec3(1.0f));
			player->SetScale(glm::vec3(startSize));
			player->SetSize(player->GetScale().x * 10.0f);

			player->Add<MovementComponent>();
			player->Add<AbilityComponent>();
			player->Add<MorphTarget>();
			player->Add<ScoreKeep>();

			FMODStudio::Sptr studio = player->Add<FMODStudio>();
			studio->engine->Init();
			studio->engine->LoadSound("Eat", "sounds/Eat.mp3", true);
			studio->engine->LoadSound("Hit", "sounds/Hit.mp3", true);
			studio->engine->LoadSound("Move", "sounds/Move.mp3", true);

			RenderComponent::Sptr renderer = player->Add<RenderComponent>();
			renderer->SetMesh(slimeMesh);
			renderer->SetMaterial(slimeMaterial);

			RigidBody::Sptr physics = player->Add<RigidBody>(RigidBodyType::Dynamic);
			physics->AddCollider(ConvexMeshCollider::Create())->SetScale(glm::vec3(startSize));
			physics->SetAngularFactor(glm::vec3(0.0f));

		}

		GameObject::Sptr backstand = scene->CreateGameObject("Backstand");
		{
			backstand->SetPostion(glm::vec3(0.0f, 0.0f, -16.0f));
			backstand->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			backstand->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = backstand->Add<RenderComponent>();
			renderer->SetMesh(backstandMesh);
			renderer->SetMaterial(backstand_texMaterial);
		}
		GameObject::Sptr flag = scene->CreateGameObject("Flag");
		{
			flag->SetPostion(glm::vec3(0.0f, 0.0f, -16.0f));
			flag->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			flag->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = flag->Add<RenderComponent>();
			renderer->SetMesh(flagMesh);
			renderer->SetMaterial(flag_texMaterial);
		}
		GameObject::Sptr gates = scene->CreateGameObject("Gates");
		{
			gates->SetPostion(glm::vec3(0.0f, 0.0f, -16.0f));
			gates->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			gates->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = gates->Add<RenderComponent>();
			renderer->SetMesh(gatesMesh);
			renderer->SetMaterial(gate_texMaterial);
		}
		GameObject::Sptr ground = scene->CreateGameObject("Ground");
		{
			ground->SetPostion(glm::vec3(0.0f, 0.0f, 12.0f));
			ground->SetRotation(glm::vec3(90.0f, 180.0f, 180.0f));
			ground->SetScale(glm::vec3(0.8f, 0.8, 2.0f));

			RenderComponent::Sptr renderer = ground->Add<RenderComponent>();
			renderer->SetMesh(groundMesh);
			renderer->SetMaterial(sandMaterial);

			RigidBody::Sptr physics = ground->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(ConvexMeshCollider::Create())->SetScale(ground->GetScale());
		}
		GameObject::Sptr poles = scene->CreateGameObject("Poles");
		{
			poles->SetPostion(glm::vec3(0.0f, 0.0f, -16.0f));
			poles->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			poles->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = poles->Add<RenderComponent>();
			renderer->SetMesh(polesMesh);
			renderer->SetMaterial(pole_texMaterial);
		}
		GameObject::Sptr poles2 = scene->CreateGameObject("Poles2");
		{
			poles2->SetPostion(glm::vec3(0.0f, 0.0f, -16.0f));
			poles2->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			poles2->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = poles2->Add<RenderComponent>();
			renderer->SetMesh(polesMesh2);
			renderer->SetMaterial(pole_texMaterial);
		}
		GameObject::Sptr stands = scene->CreateGameObject("Stands");
		{
			stands->SetPostion(glm::vec3(0.0f, 0.0f, -16.0f));
			stands->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			stands->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = stands->Add<RenderComponent>();
			renderer->SetMesh(standsMesh2);
			renderer->SetMaterial(wood_texMaterial);
		}
		GameObject::Sptr walls = scene->CreateGameObject("Walls");
		{
			walls->SetPostion(glm::vec3(0.0f, 0.0f, -16.0f));
			walls->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			walls->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = walls->Add<RenderComponent>();
			renderer->SetMesh(wallsMesh);
			renderer->SetMaterial(wood_texMaterial);

			//RigidBody::Sptr physics = walls->Add<RigidBody>(RigidBodyType::Static);
			//physics->AddCollider(ConvexMeshCollider::Create());
		}
		GameObject::Sptr banner0 = scene->CreateGameObject("Banner0");
		{
			banner0->SetPostion(glm::vec3(21.0f, 34.0f, 3.0f));
			banner0->SetRotation(glm::vec3(90.0f, 0.0f, -90.0f));
			banner0->SetScale(glm::vec3(0.8f));

			banner0->Add<MorphTarget>();

			RenderComponent::Sptr renderer = banner0->Add<RenderComponent>();
			renderer->SetMesh(bannerMesh);
			renderer->SetMaterial(banner_texMaterial);
		}
		GameObject::Sptr banner1 = scene->CreateGameObject("Banner1");
		{
			banner1->SetPostion(glm::vec3(-3.5f, 29.0f, 3.0f));
			banner1->SetRotation(glm::vec3(90.0f, 0.0f, -45.0f));
			banner1->SetScale(glm::vec3(0.8f));

			banner1->Add<MorphTarget>();

			RenderComponent::Sptr renderer = banner1->Add<RenderComponent>();
			renderer->SetMesh(bannerMesh);
			renderer->SetMaterial(banner_texMaterial);
		}
		GameObject::Sptr banner2 = scene->CreateGameObject("Banner2");
		{
			banner2->SetPostion(glm::vec3(44.5f, 28.7f, 3.0f));
			banner2->SetRotation(glm::vec3(90.0f, 0.0f, -135.0f));
			banner2->SetScale(glm::vec3(0.8f));

			banner2->Add<MorphTarget>();

			RenderComponent::Sptr renderer = banner2->Add<RenderComponent>();
			renderer->SetMesh(bannerMesh);
			renderer->SetMaterial(banner_texMaterial);
		}
		GameObject::Sptr banner3 = scene->CreateGameObject("Banner3");
		{
			banner3->SetPostion(glm::vec3(-4.0f, 3.0f, 3.0f));
			banner3->SetRotation(glm::vec3(90.0f, 0.0f, 45.0f));
			banner3->SetScale(glm::vec3(0.8f));

			banner3->Add<MorphTarget>();

			RenderComponent::Sptr renderer = banner3->Add<RenderComponent>();
			renderer->SetMesh(bannerMesh);
			renderer->SetMaterial(banner_texMaterial);
		}
		GameObject::Sptr banner4 = scene->CreateGameObject("Banner4");
		{
			banner4->SetPostion(glm::vec3(46.0f, 2.6f, 3.0f));
			banner4->SetRotation(glm::vec3(90.0f, 0.0f, 135.0f));
			banner4->SetScale(glm::vec3(0.8f));

			banner4->Add<MorphTarget>();

			RenderComponent::Sptr renderer = banner4->Add<RenderComponent>();
			renderer->SetMesh(bannerMesh);
			renderer->SetMaterial(banner_texMaterial);
		}

		GameObject::Sptr wall0 = scene->CreateGameObject("Wall0");
		{
			wall0->SetPostion(glm::vec3(20.5f, 35.0f, 0.0f));

			RigidBody::Sptr physics = wall0->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(30.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, 8.5f, 0.0f });
		}
		GameObject::Sptr wall1 = scene->CreateGameObject("Wall1");
		{
			wall1->SetPostion(glm::vec3(45.0f, 30.0f, 0.0f));
			wall1->SetRotation(glm::vec3(0.0f, 0.0f, -45.0f));

			RigidBody::Sptr physics = wall1->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, 7.0f, 0.0f });
		}
		GameObject::Sptr wall2 = scene->CreateGameObject("Wall2");
		{
			wall2->SetPostion(glm::vec3(52.0f, 15.0f, 0.0f));
			wall2->SetRotation(glm::vec3(0.0f, 0.0f, 90.0f));

			RigidBody::Sptr physics = wall2->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, -8.0f, 0.0f });
		}
		GameObject::Sptr wall3 = scene->CreateGameObject("Wall3");
		{
			wall3->SetPostion(glm::vec3(45.0f, 0.5f, 0.0f));
			wall3->SetRotation(glm::vec3(0.0f, 0.0f, 45.0f));

			RigidBody::Sptr physics = wall3->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, -7.0f, 0.0f });
		}
		GameObject::Sptr wall4 = scene->CreateGameObject("Wall4");
		{
			wall4->SetPostion(glm::vec3(-6.5f, 27.5f, 0.0f));
			wall4->SetRotation(glm::vec3(0.0f, 0.0f, 45.0f));

			RigidBody::Sptr physics = wall4->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, 8.5f, 0.0f });
		}
		GameObject::Sptr wall5 = scene->CreateGameObject("Wall5");
		{
			wall5->SetPostion(glm::vec3(-5.5f, 2.0f, 0.0f));
			wall5->SetRotation(glm::vec3(0.0f, 0.0f, -45.0f));

			RigidBody::Sptr physics = wall5->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, -7.0f, 0.0f });
		}
		GameObject::Sptr wall6 = scene->CreateGameObject("Wall6");
		{
			wall6->SetPostion(glm::vec3(-10.5f, 15.0f, 0.0f));
			wall6->SetRotation(glm::vec3(0.0f, 0.0f, 90.0f));

			RigidBody::Sptr physics = wall6->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, 7.5f, 0.0f });
		}
		GameObject::Sptr wall7 = scene->CreateGameObject("Wall7");
		{
			wall7->SetPostion(glm::vec3(20.5f, -4.0f, 0.0f));

			RigidBody::Sptr physics = wall7->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(30.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, -8.0f, 0.0f });
		}

		GameObject::Sptr superWall0 = scene->CreateGameObject("Super Wall0");
		{
			superWall0->SetPostion(glm::vec3(20.0f, 150.0f, 0.0f));

			RigidBody::Sptr physics = superWall0->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(200.0f, 10.0f, 10.0f)))->SetPosition(glm::vec3(0.0f, 60.0f, 0.0f));
		}
		GameObject::Sptr superWall1 = scene->CreateGameObject("Super Wall1");
		{
			superWall1->SetPostion(glm::vec3(-35.0f, 95.0f, 0.0f));

			RigidBody::Sptr physics = superWall1->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 150.0f, 10.0f)))->SetPosition(glm::vec3(-113.0f, 0.0f, 0.0f));
		}
		GameObject::Sptr superWall2 = scene->CreateGameObject("Super Wall2");
		{
			superWall2->SetPostion(glm::vec3(65.0f, 95.0f, 0.0f));

			RigidBody::Sptr physics = superWall2->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 150.0f, 10.0f)))->SetPosition(glm::vec3(100.0f, 0.0f, 0.0f));
		}
		GameObject::Sptr superWall3 = scene->CreateGameObject("Super Wall3");
		{
			superWall3->SetPostion(glm::vec3(40.0f, 85.0f, 0.0f));

			RigidBody::Sptr physics = superWall3->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(50.0f, 2.0f, 10.0f)))->SetPosition(glm::vec3(65.0f, -22.0f, 0.0f));
		}
		GameObject::Sptr superWall4 = scene->CreateGameObject("Super Wall4");
		{
			superWall4->SetPostion(glm::vec3(-15.0f, 85.0f, 0.0f));

			RigidBody::Sptr physics = superWall4->Add<RigidBody>(RigidBodyType::Static);
			physics->AddCollider(BoxCollider::Create(glm::vec3(50.0f, 2.0f, 10.0f)))->SetPosition(glm::vec3(-45.0f, -22.0f, 0.0f));
		}

		GameObject::Sptr spawn0 = scene->CreateGameObject("Spawn0");
		{
			spawn0->SetPostion(glm::vec3(-3.0f, 28.5f, 0.0f));
			spawn0->SetRotation(glm::vec3(0.0f, 0.0f, 45.0f));

			TriggerVolume::Sptr trigger = spawn0->Add<TriggerVolume>();
			BoxCollider::Sptr box = BoxCollider::Create(glm::vec3(10.0f, 1.0f, 15.0f));
			box->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
			trigger->AddCollider(box);

			TriggerVolumeEnterBehaviour::Sptr test = spawn0->Add<TriggerVolumeEnterBehaviour>();
			test->SetSpawnTrigger(false);
		}
		GameObject::Sptr spawn1 = scene->CreateGameObject("Spawn1");
		{
			spawn1->SetPostion(glm::vec3(43.5f, 28.5f, 0.0f));
			spawn1->SetRotation(glm::vec3(0.0f, 0.0f, -45.0f));

			TriggerVolume::Sptr trigger = spawn1->Add<TriggerVolume>();
			BoxCollider::Sptr box = BoxCollider::Create(glm::vec3(10.0f, 1.0f, 15.0f));
			box->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
			trigger->AddCollider(box);

			TriggerVolumeEnterBehaviour::Sptr test = spawn1->Add<TriggerVolumeEnterBehaviour>();
			test->SetSpawnTrigger(false);
		}
		GameObject::Sptr spawn2 = scene->CreateGameObject("Spawn2");
		{
			spawn2->SetPostion(glm::vec3(-3.0f, 3.4f, 0.0f));
			spawn2->SetRotation(glm::vec3(0.0f, 0.0f, -45.0f));

			TriggerVolume::Sptr trigger = spawn2->Add<TriggerVolume>();
			BoxCollider::Sptr box = BoxCollider::Create(glm::vec3(10.0f, 1.0f, 15.0f));
			box->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
			trigger->AddCollider(box);

			TriggerVolumeEnterBehaviour::Sptr test = spawn2->Add<TriggerVolumeEnterBehaviour>();
			test->SetSpawnTrigger(false);
		}
		GameObject::Sptr spawn3 = scene->CreateGameObject("Spawn3");
		{
			spawn3->SetPostion(glm::vec3(43.5f, 3.4f, 0.0f));
			spawn3->SetRotation(glm::vec3(0.0f, 0.0f, 45.0f));

			TriggerVolume::Sptr trigger = spawn3->Add<TriggerVolume>();
			BoxCollider::Sptr box = BoxCollider::Create(glm::vec3(10.0f, 1.0f, 15.0f));
			box->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
			trigger->AddCollider(box);

			TriggerVolumeEnterBehaviour::Sptr test = spawn3->Add<TriggerVolumeEnterBehaviour>();
			test->SetSpawnTrigger(false);
		}

		GameObject::Sptr camp = scene->CreateGameObject("Camp");
		{
			camp->SetPostion(glm::vec3(30.0f, 25.0f, -19.5f));
			camp->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			camp->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = camp->Add<RenderComponent>();
			renderer->SetMesh(campMesh);
			renderer->SetMaterial(fire_texMaterial);

			//RigidBody::Sptr physics = camp->Add<RigidBody>(RigidBodyType::Static);
			//physics->AddCollider(ConvexMeshCollider::Create())->SetScale(camp->GetScale());
		}

		GameObject::Sptr Smokeparticles = scene->CreateGameObject("Smoke Particles");
		{
			Smokeparticles->SetPostion({ 0.0f, 115.0f, 2.0f });

			ParticleSystem::Sptr particleManager = Smokeparticles->Add<ParticleSystem>();
			particleManager->Atlas = particleTex;

			particleManager->_gravity = glm::vec3(0.0f, 0.0f, 9.81f);

			ParticleSystem::ParticleData emitter;
			emitter.Type = ParticleType::SphereEmitter;
			emitter.TexID = 2;
			emitter.Position = glm::vec3(0.0f);
			emitter.Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
			emitter.Lifetime = 0.0f;
			emitter.SphereEmitterData.Timer = 1.0f / 50.0f;
			emitter.SphereEmitterData.Velocity = 0.5f;
			emitter.SphereEmitterData.LifeRange = { 1.0f, 4.0f };
			emitter.SphereEmitterData.Radius = 1.0f;
			emitter.SphereEmitterData.SizeRange = { 1.5f, 3.5f };

			particleManager->AddEmitter(emitter);
		}

		GameObject::Sptr Destoryparticles1 = scene->CreateGameObject("Destory Particles 1");
		{
			Destoryparticles1->SetPostion({ 20.5f, 35.0f, 17.0f });

			ParticleSystem::Sptr particleManager = Destoryparticles1->Add<ParticleSystem>();
			particleManager->Atlas = particleTex;

			particleManager->_gravity = glm::vec3(0.0f, 0.0f, 9.81f);

			ParticleSystem::ParticleData emitter;
			emitter.Type = ParticleType::SphereEmitter;
			emitter.TexID = 2;
			emitter.Position = glm::vec3(0.0f);
			emitter.Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
			emitter.Lifetime = 0.0f;
			emitter.SphereEmitterData.Timer = 1.0f / 50.0f;
			emitter.SphereEmitterData.Velocity = 0.5f;
			emitter.SphereEmitterData.LifeRange = { 1.0f, 3.0f };
			emitter.SphereEmitterData.Radius = 1.0f;
			emitter.SphereEmitterData.SizeRange = { 10.0f, 16.0f };

			particleManager->AddEmitter(emitter);
		}

		GameObject::Sptr Destoryparticles2 = scene->CreateGameObject("Destory Particles 2");
		{
			Destoryparticles2->SetPostion({ 52.0f, 15.0f, 17.0f });

			ParticleSystem::Sptr particleManager = Destoryparticles2->Add<ParticleSystem>();
			particleManager->Atlas = particleTex;

			particleManager->_gravity = glm::vec3(0.0f, 0.0f, 9.81f);

			ParticleSystem::ParticleData emitter;
			emitter.Type = ParticleType::SphereEmitter;
			emitter.TexID = 2;
			emitter.Position = glm::vec3(0.0f);
			emitter.Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
			emitter.Lifetime = 0.0f;
			emitter.SphereEmitterData.Timer = 1.0f / 50.0f;
			emitter.SphereEmitterData.Velocity = 0.5f;
			emitter.SphereEmitterData.LifeRange = { 1.0f, 3.0f };
			emitter.SphereEmitterData.Radius = 1.0f;
			emitter.SphereEmitterData.SizeRange = { 10.0f, 16.0f };

			particleManager->AddEmitter(emitter);
		}

		GameObject::Sptr Destoryparticles3 = scene->CreateGameObject("Destory Particles 3");
		{
			Destoryparticles3->SetPostion({ 45.0f, 30.0f, 17.0f });

			ParticleSystem::Sptr particleManager = Destoryparticles3->Add<ParticleSystem>();
			particleManager->Atlas = particleTex;

			particleManager->_gravity = glm::vec3(0.0f, 0.0f, 9.81f);

			ParticleSystem::ParticleData emitter;
			emitter.Type = ParticleType::SphereEmitter;
			emitter.TexID = 2;
			emitter.Position = glm::vec3(0.0f);
			emitter.Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
			emitter.Lifetime = 0.0f;
			emitter.SphereEmitterData.Timer = 1.0f / 50.0f;
			emitter.SphereEmitterData.Velocity = 0.5f;
			emitter.SphereEmitterData.LifeRange = { 1.0f, 3.0f };
			emitter.SphereEmitterData.Radius = 1.0f;
			emitter.SphereEmitterData.SizeRange = { 10.0f, 16.0f };

			particleManager->AddEmitter(emitter);
		}

		GameObject::Sptr Destoryparticles4 = scene->CreateGameObject("Destory Particles 4");
		{
			Destoryparticles4->SetPostion({ -5.5f, 2.0f, 17.0f });

			ParticleSystem::Sptr particleManager = Destoryparticles4->Add<ParticleSystem>();
			particleManager->Atlas = particleTex;

			particleManager->_gravity = glm::vec3(0.0f, 0.0f, 9.81f);

			ParticleSystem::ParticleData emitter;
			emitter.Type = ParticleType::SphereEmitter;
			emitter.TexID = 2;
			emitter.Position = glm::vec3(0.0f);
			emitter.Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
			emitter.Lifetime = 0.0f;
			emitter.SphereEmitterData.Timer = 1.0f / 50.0f;
			emitter.SphereEmitterData.Velocity = 0.5f;
			emitter.SphereEmitterData.LifeRange = { 1.0f, 3.0f };
			emitter.SphereEmitterData.Radius = 1.0f;
			emitter.SphereEmitterData.SizeRange = { 10.0f, 16.0f };

			particleManager->AddEmitter(emitter);
		}

		GameObject::Sptr Destoryparticles5 = scene->CreateGameObject("Destory Particles 5");
		{
			Destoryparticles5->SetPostion({ -6.5f, 27.5f, 17.0f });

			ParticleSystem::Sptr particleManager = Destoryparticles5->Add<ParticleSystem>();
			particleManager->Atlas = particleTex;

			particleManager->_gravity = glm::vec3(0.0f, 0.0f, 9.81f);

			ParticleSystem::ParticleData emitter;
			emitter.Type = ParticleType::SphereEmitter;
			emitter.TexID = 2;
			emitter.Position = glm::vec3(0.0f);
			emitter.Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
			emitter.Lifetime = 0.0f;
			emitter.SphereEmitterData.Timer = 1.0f / 50.0f;
			emitter.SphereEmitterData.Velocity = 0.5f;
			emitter.SphereEmitterData.LifeRange = { 1.0f, 3.0f };
			emitter.SphereEmitterData.Radius = 1.0f;
			emitter.SphereEmitterData.SizeRange = { 10.0f, 16.0f };

			particleManager->AddEmitter(emitter);
		}

		GameObject::Sptr Destoryparticles6 = scene->CreateGameObject("Destory Particles 6");
		{
			Destoryparticles6->SetPostion({ -10.5f, 15.0f, 17.0f });

			ParticleSystem::Sptr particleManager = Destoryparticles6->Add<ParticleSystem>();
			particleManager->Atlas = particleTex;

			particleManager->_gravity = glm::vec3(0.0f, 0.0f, 9.81f);

			ParticleSystem::ParticleData emitter;
			emitter.Type = ParticleType::SphereEmitter;
			emitter.TexID = 2;
			emitter.Position = glm::vec3(0.0f);
			emitter.Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
			emitter.Lifetime = 0.0f;
			emitter.SphereEmitterData.Timer = 1.0f / 50.0f;
			emitter.SphereEmitterData.Velocity = 0.5f;
			emitter.SphereEmitterData.LifeRange = { 1.0f, 3.0f };
			emitter.SphereEmitterData.Radius = 1.0f;
			emitter.SphereEmitterData.SizeRange = { 10.0f, 16.0f };

			particleManager->AddEmitter(emitter);
		}

		GameObject::Sptr Destoryparticles7 = scene->CreateGameObject("Destory Particles 7");
		{
			Destoryparticles7->SetPostion({ -5.5f, 2.0f, 17.0f });

			ParticleSystem::Sptr particleManager = Destoryparticles7->Add<ParticleSystem>();
			particleManager->Atlas = particleTex;

			particleManager->_gravity = glm::vec3(0.0f, 0.0f, 9.81f);

			ParticleSystem::ParticleData emitter;
			emitter.Type = ParticleType::SphereEmitter;
			emitter.TexID = 2;
			emitter.Position = glm::vec3(0.0f);
			emitter.Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
			emitter.Lifetime = 0.0f;
			emitter.SphereEmitterData.Timer = 1.0f / 50.0f;
			emitter.SphereEmitterData.Velocity = 0.5f;
			emitter.SphereEmitterData.LifeRange = { 1.0f, 3.0f };
			emitter.SphereEmitterData.Radius = 1.0f;
			emitter.SphereEmitterData.SizeRange = { 10.0f, 16.0f };

			particleManager->AddEmitter(emitter);
		}

		GameObject::Sptr doubleRock = scene->CreateGameObject("Double Rock");
		{
			doubleRock->SetPostion(glm::vec3(0.0f, 25.0f, -20.0f));
			doubleRock->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			doubleRock->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = doubleRock->Add<RenderComponent>();
			renderer->SetMesh(doubleRockMesh);
			renderer->SetMaterial(rock_texMaterial);

			//RigidBody::Sptr physics = doubleRock->Add<RigidBody>(RigidBodyType::Static);
			//physics->AddCollider(ConvexMeshCollider::Create())->SetScale(doubleRock->GetScale());
		}
		GameObject::Sptr longRock = scene->CreateGameObject("Long Rock");
		{
			longRock->SetPostion(glm::vec3(135.0f, 25.0f, 13.5f));
			longRock->SetRotation(glm::vec3(90.0f, 180.0f, 180.0f));
			longRock->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = longRock->Add<RenderComponent>();
			renderer->SetMesh(longRockMesh);
			renderer->SetMaterial(rock_texMaterial);

			//RigidBody::Sptr physics = longRock->Add<RigidBody>(RigidBodyType::Static);
			//physics->AddCollider(ConvexMeshCollider::Create())->SetScale(longRock->GetScale());
		}
		GameObject::Sptr mountain = scene->CreateGameObject("Mountain");
		{
			mountain->SetPostion(glm::vec3(0.0f, 25.0f, -17.5f));
			mountain->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			mountain->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = mountain->Add<RenderComponent>();
			renderer->SetMesh(envMountainMesh);
			renderer->SetMaterial(mountain_texMaterial);

			//RigidBody::Sptr physics = mountain->Add<RigidBody>(RigidBodyType::Static);
			//physics->AddCollider(ConvexMeshCollider::Create())->SetScale(mountain->GetScale());
		}
		GameObject::Sptr fence = scene->CreateGameObject("Fence");
		{
			fence->SetPostion(glm::vec3(0.0f, 25.0f, -17.5f));
			fence->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			fence->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = fence->Add<RenderComponent>();
			renderer->SetMesh(fenceMesh2);
			renderer->SetMaterial(fence_texMaterial);

			//RigidBody::Sptr physics = fence->Add<RigidBody>(RigidBodyType::Static);
			//physics->AddCollider(ConvexMeshCollider::Create())->SetScale(fence->GetScale());
		}
		GameObject::Sptr orcSpawn0 = scene->CreateGameObject("Orc Spawn0");
		{
			orcSpawn0->SetPostion(glm::vec3(-15.0f, 75.0f, -17.5f));
			orcSpawn0->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			orcSpawn0->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = orcSpawn0->Add<RenderComponent>();
			renderer->SetMesh(orcSpawnMesh);
			renderer->SetMaterial(orc_spawnTexMaterial);

			//RigidBody::Sptr physics = orcSpawn0->Add<RigidBody>(RigidBodyType::Static);
			//physics->AddCollider(ConvexMeshCollider::Create())->SetScale(orcSpawn0->GetScale());
		}
		GameObject::Sptr orcSpawn1 = scene->CreateGameObject("Orc Spawn1");
		{
			orcSpawn1->SetPostion(glm::vec3(-15.0f, -25.0f, -17.5f));
			orcSpawn1->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			orcSpawn1->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = orcSpawn1->Add<RenderComponent>();
			renderer->SetMesh(orcSpawnMesh);
			renderer->SetMaterial(orc_spawnTexMaterial);

			//RigidBody::Sptr physics = orcSpawn1->Add<RigidBody>(RigidBodyType::Static);
			//physics->AddCollider(ConvexMeshCollider::Create())->SetScale(orcSpawn1->GetScale());
		}
		GameObject::Sptr orcSpawn2 = scene->CreateGameObject("Orc Spawn2");
		{
			orcSpawn2->SetPostion(glm::vec3(-245.0f, 75.0f, -17.5f));
			orcSpawn2->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			orcSpawn2->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = orcSpawn2->Add<RenderComponent>();
			renderer->SetMesh(orcSpawnMesh);
			renderer->SetMaterial(orc_spawnTexMaterial);

			//RigidBody::Sptr physics = orcSpawn2->Add<RigidBody>(RigidBodyType::Static);
			//physics->AddCollider(ConvexMeshCollider::Create())->SetScale(orcSpawn2->GetScale());
		}
		GameObject::Sptr orcSpawn3 = scene->CreateGameObject("Orc Spawn3");
		{
			orcSpawn3->SetPostion(glm::vec3(-245.0f, -25.0f, -17.5f));
			orcSpawn3->SetRotation(glm::vec3(90.0f, 0.0f, 180.0f));
			orcSpawn3->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = orcSpawn3->Add<RenderComponent>();
			renderer->SetMesh(orcSpawnMesh);
			renderer->SetMaterial(orc_spawnTexMaterial);

			//RigidBody::Sptr physics = orcSpawn3->Add<RigidBody>(RigidBodyType::Static);
			//physics->AddCollider(ConvexMeshCollider::Create())->SetScale(orcSpawn3->GetScale());
		}

		GameObject::Sptr superSpawn0 = scene->CreateGameObject("Super Spawn0");
		{
			superSpawn0->SetPostion(glm::vec3(-80.0f, 180.0f, 0.0f));
			superSpawn0->SetRotation(glm::vec3(0.0f, 0.0f, 45.0f));

			TriggerVolume::Sptr trigger = superSpawn0->Add<TriggerVolume>();
			BoxCollider::Sptr box = BoxCollider::Create(glm::vec3(10.0f, 1.0f, 15.0f));
			box->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
			trigger->AddCollider(box);

			TriggerVolumeEnterBehaviour::Sptr test = superSpawn0->Add<TriggerVolumeEnterBehaviour>();
			test->SetSpawnTrigger(false);
		}
		GameObject::Sptr superSpawn1 = scene->CreateGameObject("Super Spawn1");
		{
			superSpawn1->SetPostion(glm::vec3(115.0f, 180.0f, 0.0f));
			superSpawn1->SetRotation(glm::vec3(0.0f, 0.0f, -45.0f));

			TriggerVolume::Sptr trigger = superSpawn1->Add<TriggerVolume>();
			BoxCollider::Sptr box = BoxCollider::Create(glm::vec3(10.0f, 1.0f, 15.0f));
			box->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
			trigger->AddCollider(box);

			TriggerVolumeEnterBehaviour::Sptr test = superSpawn1->Add<TriggerVolumeEnterBehaviour>();
			test->SetSpawnTrigger(false);
		}
		GameObject::Sptr superSpawn2 = scene->CreateGameObject("Super Spawn2");
		{
			superSpawn2->SetPostion(glm::vec3(120.0f, 90.0f, 0.0f));
			superSpawn2->SetRotation(glm::vec3(0.0f, 0.0f, 45.0f));

			TriggerVolume::Sptr trigger = superSpawn2->Add<TriggerVolume>();
			BoxCollider::Sptr box = BoxCollider::Create(glm::vec3(10.0f, 1.0f, 15.0f));
			box->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
			trigger->AddCollider(box);

			TriggerVolumeEnterBehaviour::Sptr test = superSpawn2->Add<TriggerVolumeEnterBehaviour>();
			test->SetSpawnTrigger(false);
		}
		GameObject::Sptr superSpawn3 = scene->CreateGameObject("Super Spawn3");
		{
			superSpawn3->SetPostion(glm::vec3(-80.0f, 90.0f, 0.0f));
			superSpawn3->SetRotation(glm::vec3(0.0f, 0.0f, -45.0f));

			TriggerVolume::Sptr trigger = superSpawn3->Add<TriggerVolume>();
			BoxCollider::Sptr box = BoxCollider::Create(glm::vec3(10.0f, 1.0f, 15.0f));
			box->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
			trigger->AddCollider(box);

			TriggerVolumeEnterBehaviour::Sptr test = superSpawn3->Add<TriggerVolumeEnterBehaviour>();
			test->SetSpawnTrigger(false);
		}

		GameObject::Sptr pausePanel = scene->CreateGameObject("Pause Panel");
		{
			RectTransform::Sptr transform = pausePanel->Add<RectTransform>();
			transform->SetMin({ -12, -12 });
			transform->SetMax({ 2000, 2000 });

			GuiPanel::Sptr panel = pausePanel->Add<GuiPanel>();
			panel->SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			pausePanel->Add<PauseComponent>();

			GameObject::Sptr titleText = scene->CreateGameObject("Title Text");
			{
				RectTransform::Sptr transform = titleText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x / 2, -100 });

				GuiPanel::Sptr panel = titleText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 35.0f);
				font->Bake();

				GuiText::Sptr text = titleText->Add<GuiText>();
				text->SetText("Paused");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr resumeText = scene->CreateGameObject("Resume Text");
			{
				RectTransform::Sptr transform = resumeText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x / 2, -100 });

				GuiPanel::Sptr panel = resumeText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = resumeText->Add<GuiText>();
				text->SetText("Resume");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr exitText = scene->CreateGameObject("Exit Text");
			{
				RectTransform::Sptr transform = exitText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x / 2, -100 });

				GuiPanel::Sptr panel = exitText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = exitText->Add<GuiText>();
				text->SetText("Exit");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr scoreText = scene->CreateGameObject("Score Text");
			{
				RectTransform::Sptr transform = scoreText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x / 2, 1000 });

				GuiPanel::Sptr panel = scoreText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = scoreText->Add<GuiText>();
				text->SetText("");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			pausePanel->AddChild(titleText);
			pausePanel->AddChild(resumeText);
			pausePanel->AddChild(exitText);
			pausePanel->AddChild(scoreText);
		}

		GameObject::Sptr tutorial = scene->CreateGameObject("Tutorial");
		{
			RectTransform::Sptr transform = tutorial->Add<RectTransform>();
			transform->SetMin({ -12, -12 });
			transform->SetMax({ 2000, 2000 });

			GuiPanel::Sptr panel = tutorial->Add<GuiPanel>();
			panel->SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.25f));

			GameObject::Sptr tutorialPanel = scene->CreateGameObject("Tutorial Panel");
			{
				RectTransform::Sptr transform = tutorialPanel->Add<RectTransform>();
				transform->SetMin({ 525, 313 });
				transform->SetMax({ 1146, 660 });
				transform->SetPosition({ app.GetWindowSize().x / 2, app.GetWindowSize().y / 2 });

				GuiPanel::Sptr panel = tutorialPanel->Add<GuiPanel>();
				panel->SetColor(glm::vec4(1.0f));

				panel->SetTexture(steps.front());
			}

			GameObject::Sptr nextText = scene->CreateGameObject("Next Text");
			{
				RectTransform::Sptr transform = nextText->Add<RectTransform>();
				transform->SetMin({ 830, 696 });
				transform->SetMax({ 839, 696 });
				transform->SetPosition({ app.GetWindowSize().x / 2, app.GetWindowSize().y / 2 + 150 });

				GuiPanel::Sptr panel = nextText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 35.0f);
				font->Bake();

				GuiText::Sptr text = nextText->Add<GuiText>();
				text->SetText("Next");
				text->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				text->SetFont(font);
			}

			/*
			GameObject::Sptr backText = scene->CreateGameObject("Back Text");
			{
				RectTransform::Sptr transform = backText->Add<RectTransform>();
				transform->SetMin({ -10, 270 });
				transform->SetMax({ 60, 270 });
				//transform->SetPosition({ app.GetWindowSize().x / 2,  app.GetWindowSize().y / 2 });

				GuiPanel::Sptr panel = backText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 35.0f);
				font->Bake();

				GuiText::Sptr text = backText->Add<GuiText>();
				text->SetText("Back");
				text->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
				text->SetFont(font);
			}
			*/

			GameObject::Sptr skipText = scene->CreateGameObject("Skip Text");
			{
				RectTransform::Sptr transform = skipText->Add<RectTransform>();
				transform->SetMin({ 1088, 275 });
				transform->SetMax({ 1132, 324 });
				transform->SetPosition({ app.GetWindowSize().x / 2 + 100,  app.GetWindowSize().y / 2 - 150 });

				GuiPanel::Sptr panel = skipText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 35.0f);
				font->Bake();

				GuiText::Sptr text = skipText->Add<GuiText>();
				text->SetText("X");
				text->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				text->SetFont(font);
			}

			tutorial->AddChild(tutorialPanel);
			tutorial->AddChild(nextText);
			//tutorial->AddChild(backText);
			tutorial->AddChild(skipText);
		}

		GameObject::Sptr shadowMap = scene->CreateGameObject("Shadow Map");
		{
			shadowMap->SetPostion(glm::vec3(0.0f, 160.0f, 90.0f));
			shadowMap->LookAt(player->GetPosition());
			shadowMap->SetRotation(glm::vec3(-25.0f, -4.0f, 0.0f));

			ShadowCamera::Sptr shadowCam = shadowMap->Add<ShadowCamera>();
			shadowCam->SetProjection(glm::perspective(glm::radians(120.0f), 1.0f, 0.1f, 200.0f));
			shadowCam->Intensity = 1.5f;
		}

		// Create sound
		engine->Init();
		engine->LoadSound("Tutorial 1", "sounds/Tutorial_Text_1.wav", true);
		engine->LoadSound("Tutorial 2", "sounds/Tutorial_Text_2.wav", true);
		engine->LoadSound("Tutorial 3", "sounds/Tutorial_Text_3.wav", true);
		engine->LoadSound("Theme 1", "sounds/Enter The Arena.wav", true);
		engine->LoadSound("Theme 2", "sounds/Into The Wild.wav", true);

		GuiBatcher::SetDefaultTexture(ResourceManager::CreateAsset<Texture2D>("textures/ui-sprite.png"));
		GuiBatcher::SetDefaultBorderRadius(8);

		// Save the asset manifest for all the resources we just loaded
		ResourceManager::SaveManifest("main-game-manifest.json");
		
		// Save the scene to a JSON file
		scene->Save("scenes/main-game.json");

		// Send the scene to the application
		app.LoadScene(scene);
	}
}

void MainSceneLayer::_MainLoop()
{
	Application& app = Application::Get();
	Scene::Sptr scene = app.CurrentScene();
	Timing& time = Timing::Current();

	GameObject::Sptr player = scene->FindObjectByName("Player");

	GameObject::Sptr backstand = scene->FindObjectByName("Backstand");
	GameObject::Sptr flag = scene->FindObjectByName("Flag");
	GameObject::Sptr gates = scene->FindObjectByName("Gates");
	GameObject::Sptr poles = scene->FindObjectByName("Poles");
	GameObject::Sptr poles2 = scene->FindObjectByName("Poles2");
	GameObject::Sptr stands = scene->FindObjectByName("Stands");
	GameObject::Sptr wall = scene->FindObjectByName("Walls");
	GameObject::Sptr banner0 = scene->FindObjectByName("Banner0");
	GameObject::Sptr banner1 = scene->FindObjectByName("Banner1");
	GameObject::Sptr banner2 = scene->FindObjectByName("Banner2");
	GameObject::Sptr banner3 = scene->FindObjectByName("Banner3");
	GameObject::Sptr banner4 = scene->FindObjectByName("Banner4");

	GameObject::Sptr shadowMap = scene->FindObjectByName("Shadow Map");

	GameObject::Sptr pausePanel = scene->FindObjectByName("Pause Panel");
	GameObject::Sptr resumeText = scene->FindObjectByName("Resume Text");
	GameObject::Sptr exitText = scene->FindObjectByName("Exit Text");
	GameObject::Sptr scoreText = scene->FindObjectByName("Score Text");

	GameObject::Sptr tutorial = scene->FindObjectByName("Tutorial");
	GameObject::Sptr tutorialPanel = scene->FindObjectByName("Tutorial Panel");
	GameObject::Sptr nextText = scene->FindObjectByName("Next Text");
	//GameObject::Sptr backText = scene->FindObjectByName("Back Text");
	GameObject::Sptr skipText = scene->FindObjectByName("Skip Text");

	GameObject::Sptr DestoryParticles1 = scene->FindObjectByName("Destory Particles 1");
	GameObject::Sptr DestoryParticles2 = scene->FindObjectByName("Destory Particles 2");
	GameObject::Sptr DestoryParticles3 = scene->FindObjectByName("Destory Particles 3");
	GameObject::Sptr DestoryParticles4 = scene->FindObjectByName("Destory Particles 4");
	GameObject::Sptr DestoryParticles5 = scene->FindObjectByName("Destory Particles 5");
	GameObject::Sptr DestoryParticles6 = scene->FindObjectByName("Destory Particles 6");
	GameObject::Sptr DestoryParticles7 = scene->FindObjectByName("Destory Particles 7");

	glm::vec2 mousePosition = glm::vec2(InputEngine::GetMousePos().x, InputEngine::GetMousePos().y);

	shadowMap->Get<ShadowCamera>()->Amount = app.GetLayer<MenuSceneLayer>()->shadowMapping ? 1.0f : 0.0f;
	player->Get<FMODStudio>()->volume = app.GetLayer<MenuSceneLayer>()->effectVolume;
	shadowMap->Get<ShadowCamera>()->IsEnabled = app.GetLayer<MenuSceneLayer>()->lightingToggle;

	TutorialButtons(tutorial, tutorialPanel, nextText, skipText, mousePosition);

	int destroyWallSize = 35;

	// Calling wall objects
	std::vector<GameObject::Sptr> walls;
	{
		walls.resize(wallAmount);
		for (size_t i = 0; i < wallAmount; i++)
		{
			walls[i] = scene->FindObjectByName("Wall" + std::to_string(i));
		}
	}

	// Calling super wall objects
	std::vector<GameObject::Sptr> superWalls;
	{
		superWalls.resize(superWallAmount);
		for (size_t i = 0; i < superWallAmount; i++)
		{
			superWalls[i] = scene->FindObjectByName("Super Wall" + std::to_string(i));
		}
	}

	// Turning on and off the wall in arena
	if (player->Get<ScoreKeep>()->bestSize >= destroyWallSize && !deleteWall)
	{
		theme.SetPause(true);
		theme.SetSound("Theme 2");
		theme.Play();
		theme.SetVolume(app.GetLayer<MenuSceneLayer>()->musicVolume);
		theme.SetLoop(true);
		theme.SetPause(false);

		for (size_t i = 0; i < wallAmount; i++)
		{
			walls[i]->Get<RigidBody>()->RemoveCollider(walls[i]->Get<RigidBody>()->GetCollider(0));
		}
		deleteWall = true;
	}
	if (player->Get<ScoreKeep>()->bestSize < destroyWallSize && deleteWall)
	{
		walls[0]->Get<RigidBody>()->AddCollider(BoxCollider::Create(glm::vec3(30.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, 8.5f, 0.0f });
		walls[1]->Get<RigidBody>()->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, 7.0f, 0.0f });
		walls[2]->Get<RigidBody>()->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, -8.0f, 0.0f });
		walls[3]->Get<RigidBody>()->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, -7.0f, 0.0f });
		walls[4]->Get<RigidBody>()->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, 8.5f, 0.0f });
		walls[5]->Get<RigidBody>()->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, -7.0f, 0.0f });
		walls[6]->Get<RigidBody>()->AddCollider(BoxCollider::Create(glm::vec3(10.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, 7.5f, 0.0f });
		walls[7]->Get<RigidBody>()->AddCollider(BoxCollider::Create(glm::vec3(30.0f, 10.0f, 10.0f)))->SetPosition({ 0.0f, -8.0f, 0.0f });
		arenaLerp = 0.0f;
		deleteWall = false;
	}

	// Calling spawn objects
	std::vector<GameObject::Sptr> spawns;
	{
		spawns.resize(4);
		for (size_t i = 0; i < 4; i++)
		{
			spawns[i] = scene->FindObjectByName("Spawn" + std::to_string(i));
		}
	}

	// Calling super spawn objects
	std::vector<GameObject::Sptr> superSpawns;
	{
		superSpawns.resize(4);
		for (size_t i = 0; i < 4; i++)
		{
			superSpawns[i] = scene->FindObjectByName("Super Spawn" + std::to_string(i));
		}
	}

	// Banner animations
	std::vector<GameObject::Sptr> banners;
	{
		banners.resize(5);
		for (size_t i = 0; i < 5; i++)
		{
			banners[i] = scene->FindObjectByName("Banner" + std::to_string(i));
			bannerWind.UpdateAnimation(banners[i], time.DeltaTime());
		}
	}

	// Updating score UI
	std::string scoreString = "Enemies Eaten: " + std::to_string(int(player->Score.x)) + "\t\tTime Played: " + std::to_string(int(player->Score.y)) + "\t\tSlime Size: " + std::to_string(int(player->Score.z));
	scoreText->Get<GuiText>()->SetText(scoreString);

	// Spawning enemies
	if (player->Score.y > player->Get<ScoreKeep>()->nextSpawn)
	{
		// Count how many regular enemies are playing
		for (size_t i = 0; i < enemyAmount; i++)
		{
			GameObject::Sptr tempEnemy = scene->FindObjectByName("Enemy" + std::to_string(i));
			if (tempEnemy != nullptr)
			{
				player->Get<ScoreKeep>()->enemyCounter++;
			}
		}

		// Count how many super enemies are playing
		for (size_t i = 0; i < superEnemyAmount; i++)
		{
			GameObject::Sptr tempEnemy = scene->FindObjectByName("Super Enemy" + std::to_string(i));
			if (tempEnemy != nullptr)
			{
				player->Get<ScoreKeep>()->superEnemyCounter++;
			}
		}

		// Spawn regular enemies
		if (!deleteWall && player->Get<ScoreKeep>()->enemyCounter <= 8)
		{
			CreateEnemies(scene, spawns, player, walls);
		}

		// Spawn super enemies
		if (deleteWall && player->Get<ScoreKeep>()->superEnemyCounter <= 8)
		{
			CreateSuperEnemies(scene, superSpawns, player, superWalls);
		}

		player->Get<ScoreKeep>()->enemyCounter = 0;
		player->Get<ScoreKeep>()->superEnemyCounter = 0;
		player->Get<ScoreKeep>()->nextSpawn = player->Score.y + player->Get<ScoreKeep>()->spawnCooldown;
	}

	// Enemy animations
	std::vector<GameObject::Sptr> enemy;
	{
		enemy.resize(enemyAmount);
		for (size_t i = 0; i < enemyAmount; i++)
		{
			enemy[i] = scene->FindObjectByName("Enemy" + std::to_string(i));
			if (enemy[i] != nullptr)
			{
				if (enemy[i]->Get<MorphTarget>()->GetType() == AnimationType::Walk)
				{
					enemyWalk.UpdateAnimation(enemy[i], time.DeltaTime());
					enemy[i]->Get<RenderComponent>()->SetMaterial(enemyMaterial);
				}
				if (enemy[i]->Get<MorphTarget>()->GetType() == AnimationType::Attack)
				{
					enemyAttack.UpdateAnimation(enemy[i], time.DeltaTime());
					enemy[i]->Get<RenderComponent>()->SetMaterial(attackMaterial);
				}
			}
		}
	}

	// Super enemy animations
	std::vector<GameObject::Sptr> superEnemy;
	{
		superEnemy.resize(superEnemyAmount);
		for (size_t i = 0; i < superEnemyAmount; i++)
		{
			superEnemy[i] = scene->FindObjectByName("Super Enemy" + std::to_string(i));
			if (superEnemy[i] != nullptr)
			{
				if (superEnemy[i]->Get<MorphTarget>()->GetType() == AnimationType::Walk)
				{
					superEnemyWalk.UpdateAnimation(superEnemy[i], time.DeltaTime());
					superEnemy[i]->Get<RenderComponent>()->SetMaterial(superEnemyMaterial);
				}
				if (superEnemy[i]->Get<MorphTarget>()->GetType() == AnimationType::Attack)
				{
					superEnemyAttack.UpdateAnimation(superEnemy[i], time.DeltaTime());
					superEnemy[i]->Get<RenderComponent>()->SetMaterial(attackMaterial);
				}
			}
		}
	}

	// Arena destroyed
	if (deleteWall && backstand->GetPosition().z > -50.0f)
	{
		glm::vec3 backstandStart = backstand->GetPosition();
		glm::vec3 flagStart = flag->GetPosition();
		glm::vec3 gateStart = gates->GetPosition();
		glm::vec3 polesStart = poles->GetPosition();
		glm::vec3 poles2Start = poles2->GetPosition();
		glm::vec3 standsStart = stands->GetPosition();
		glm::vec3 wallStart = wall->GetPosition();
		glm::vec3 banner0Start = banner0->GetPosition();
		glm::vec3 banner1Start = banner1->GetPosition();
		glm::vec3 banner2Start = banner2->GetPosition();
		glm::vec3 banner3Start = banner3->GetPosition();
		glm::vec3 banner4Start = banner4->GetPosition();
		glm::vec3 DestoryParticles1Start = DestoryParticles1->GetPosition();
		glm::vec3 DestoryParticles2Start = DestoryParticles2->GetPosition();
		glm::vec3 DestoryParticles3Start = DestoryParticles3->GetPosition();
		glm::vec3 DestoryParticles4Start = DestoryParticles4->GetPosition();
		glm::vec3 DestoryParticles5Start = DestoryParticles5->GetPosition();
		glm::vec3 DestoryParticles6Start = DestoryParticles6->GetPosition();
		glm::vec3 DestoryParticles7Start = DestoryParticles7->GetPosition();

		arenaLerp += time.DeltaTime() / 25;
		float rand = glm::linearRand(-arenaLerp, arenaLerp);
		rand *= 7.5f;

		backstand->SetPostion(backstandStart + glm::vec3(sin(rand), 0.0f, 0.0f));
		flag->SetPostion(flagStart + glm::vec3(sin(rand), 0.0f, 0.0f));
		gates->SetPostion(gateStart + glm::vec3(sin(rand), 0.0f, 0.0f));
		poles->SetPostion(polesStart + glm::vec3(sin(rand), 0.0f, 0.0f));
		poles2->SetPostion(poles2Start + glm::vec3(sin(rand), 0.0f, 0.0f));
		stands->SetPostion(standsStart + glm::vec3(sin(rand), 0.0f, 0.0f));
		wall->SetPostion(wallStart + glm::vec3(sin(rand), 0.0f, 0.0f));
		banner0->SetPostion(banner0Start + glm::vec3(sin(rand), 0.0f, 0.0f));
		banner1->SetPostion(banner1Start + glm::vec3(sin(rand), 0.0f, 0.0f));
		banner2->SetPostion(banner2Start + glm::vec3(sin(rand), 0.0f, 0.0f));
		banner3->SetPostion(banner3Start + glm::vec3(sin(rand), 0.0f, 0.0f));
		banner4->SetPostion(banner4Start + glm::vec3(sin(rand), 0.0f, 0.0f));
		DestoryParticles1->SetPostion(DestoryParticles1Start + glm::vec3(sin(rand), 0.0f, 0.0f));
		DestoryParticles2->SetPostion(DestoryParticles2Start + glm::vec3(sin(rand), 0.0f, 0.0f));
		DestoryParticles3->SetPostion(DestoryParticles3Start + glm::vec3(sin(rand), 0.0f, 0.0f));
		DestoryParticles4->SetPostion(DestoryParticles4Start + glm::vec3(sin(rand), 0.0f, 0.0f));
		DestoryParticles5->SetPostion(DestoryParticles5Start + glm::vec3(sin(rand), 0.0f, 0.0f));
		DestoryParticles6->SetPostion(DestoryParticles6Start + glm::vec3(sin(rand), 0.0f, 0.0f));
		DestoryParticles7->SetPostion(DestoryParticles7Start + glm::vec3(sin(rand), 0.0f, 0.0f));

		backstand->SetPostion(backstand->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		flag->SetPostion(flag->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		gates->SetPostion(gates->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		poles->SetPostion(poles->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		poles2->SetPostion(poles2->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		stands->SetPostion(stands->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		wall->SetPostion(wall->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		banner0->SetPostion(banner0->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		banner1->SetPostion(banner1->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		banner2->SetPostion(banner2->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		banner3->SetPostion(banner3->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		banner4->SetPostion(banner4->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		DestoryParticles1->SetPostion(DestoryParticles1->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		DestoryParticles2->SetPostion(DestoryParticles2->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		DestoryParticles3->SetPostion(DestoryParticles3->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		DestoryParticles4->SetPostion(DestoryParticles4->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		DestoryParticles5->SetPostion(DestoryParticles5->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		DestoryParticles6->SetPostion(DestoryParticles6->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
		DestoryParticles7->SetPostion(DestoryParticles7->GetPosition() - glm::vec3(0.0f, 0.0f, arenaLerp));
	}

	// Button functions
	//ParticlesMove(player, particles, time.DeltaTime());

	ResumeButton(pausePanel, resumeText, mousePosition);
	ExitButton(player, pausePanel, exitText, mousePosition);

	// Slime animation
	if (!player->Get<MovementComponent>()->isIdle)
	{
		slimeMove.UpdateAnimation(player, time.DeltaTime());
	}

	// Top down camera
	if (player->GetSize() >= destroyWallSize && !cameraShift)
	{
		if (cameraLerp < 1.0f)
		{
			cameraLerp += time.DeltaTime();
			cameraHeight = LERP(15.0f, 20.0f, cameraLerp);
			cameraDistance = LERP(8.0f, 13.0f, cameraLerp);
		}
		else
		{
			cameraLerp = 0.1f;
			cameraShift = true;
		}
	}
	if (player->GetSize() < destroyWallSize && cameraShift)
	{
		if (cameraLerp < 1.0f)
		{
			cameraLerp += time.DeltaTime();
			cameraHeight = LERP(20.0f, 15.0f, cameraLerp);
			cameraDistance = LERP(13.0f, 8.0f, cameraLerp);
		}
		else
		{
			cameraLerp = 0.1f;
			cameraShift = false;
		}
	}
	Camera::Sptr camera = scene->MainCamera;
	GameObject* cam = camera->GetGameObject();
	TopDownCamera(cam, player);

	engine->Update();
}