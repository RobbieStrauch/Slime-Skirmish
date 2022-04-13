// GLM math library
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/common.hpp> // for fmod (floating modulus)
#include "GLM/gtx/matrix_decompose.hpp"
#include <GLM/gtc/random.hpp>

#include <filesystem>
#include <random>
#include <memory>

#include "Logging.h"
#include "imgui.h"

#include "MenuSceneLayer.h"

using namespace Gameplay;
using namespace Gameplay::Physics;

MenuSceneLayer::MenuSceneLayer() : ApplicationLayer()
{
	Name = "Menu Scene";
	Overrides = AppLayerFunctions::OnAppLoad | AppLayerFunctions::OnUpdate;
}

MenuSceneLayer::~MenuSceneLayer() = default;

void MenuSceneLayer::OnAppLoad(const nlohmann::json& config)
{
	_CreateScene();
}

void MenuSceneLayer::OnUpdate()
{
	if (Application::Get().CurrentScene()->FindObjectByName("Menu Panel") != nullptr)
	{
		_MainLoop();
	}
}

void MenuSceneLayer::DeclareButton(GameObject::Sptr textPanel, glm::vec2 mouse)
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

void MenuSceneLayer::DeclareButton(GameObject::Sptr textPanel, glm::vec2 mouse, bool shorter)
{
	double distance = glm::distance(mouse, textPanel->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance - 15)
	{
		textPanel->Get<GuiText>()->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		textPanel->Get<GuiText>()->SetColor(glm::vec4(1.0f));
	}
}

void MenuSceneLayer::StartButton(GameObject::Sptr startButton, glm::vec2 mouse)
{
	DeclareButton(startButton, mouse);
	double distance = glm::distance(mouse, startButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed && std::filesystem::exists("scenes/main-game.json"))
	{
		Application::Get().LoadScene("scenes/main-game.json");
	}
}

void MenuSceneLayer::OptionsButton(GameObject::Sptr optionsButton, glm::vec2 mouse)
{
	DeclareButton(optionsButton, mouse);
	double distance = glm::distance(mouse, optionsButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		options = true;
		pressed = true;
	}
}

void MenuSceneLayer::ExitButton(GameObject::Sptr exitButton, glm::vec2 mouse)
{
	DeclareButton(exitButton, mouse);
	double distance = glm::distance(mouse, exitButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		Application::Get().Quit();
	}
}

void MenuSceneLayer::BackButton(GameObject::Sptr backButton, glm::vec2 mouse)
{
	DeclareButton(backButton, mouse);
	double distance = glm::distance(mouse, backButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed && options)
	{
		options = false;
		pressed = true;
	}
	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed && sounds)
	{
		sounds = false;
		options = true;
		pressed = true;
	}
	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed && graphics)
	{
		graphics = false;
		options = true;
		pressed = true;
	}
	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed && controls)
	{
		controls = false;
		options = true;
		pressed = true;
	}
}

void MenuSceneLayer::SoundButton(GameObject::Sptr soundButton, glm::vec2 mouse)
{
	DeclareButton(soundButton, mouse);
	double distance = glm::distance(mouse, soundButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		sounds = true;
		options = false;
		pressed = true;
	}
}

void MenuSceneLayer::VolumeButtons(GameObject::Sptr volumeText, GameObject::Sptr volumeMinus, GameObject::Sptr volumePlus, glm::vec2 mouse)
{
	DeclareButton(volumeMinus, mouse);
	DeclareButton(volumePlus, mouse);
	double distanceMinus = glm::distance(mouse, volumeMinus->Get<RectTransform>()->GetPosition());
	double distancePlus = glm::distance(mouse, volumePlus->Get<RectTransform>()->GetPosition());

	if (distanceMinus < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed && musicVolume > 0)
	{
		musicVolume -= 1;
	}
	if (distancePlus < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed && musicVolume < 10)
	{
		musicVolume += 1;
	}
}

void MenuSceneLayer::EffectVolumeButtons(GameObject::Sptr effectVolumeText, GameObject::Sptr effectVolumeMinus, GameObject::Sptr effectVolumePlus, glm::vec2 mouse)
{
	DeclareButton(effectVolumeMinus, mouse);
	DeclareButton(effectVolumePlus, mouse);
	double distanceMinus = glm::distance(mouse, effectVolumeMinus->Get<RectTransform>()->GetPosition());
	double distancePlus = glm::distance(mouse, effectVolumePlus->Get<RectTransform>()->GetPosition());

	if (distanceMinus < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed && effectVolume > 0)
	{
		effectVolume -= 1;
	}
	if (distancePlus < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed && effectVolume < 10)
	{
		effectVolume += 1;
	}
}

void MenuSceneLayer::DialogueVolumeButtons(GameObject::Sptr dialogueVolumeText, GameObject::Sptr dialogueVolumeMinus, GameObject::Sptr dialogueVolumePlus, glm::vec2 mouse)
{
	DeclareButton(dialogueVolumeMinus, mouse);
	DeclareButton(dialogueVolumePlus, mouse);
	double distanceMinus = glm::distance(mouse, dialogueVolumeMinus->Get<RectTransform>()->GetPosition());
	double distancePlus = glm::distance(mouse, dialogueVolumePlus->Get<RectTransform>()->GetPosition());

	if (distanceMinus < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed && dialogueVolume > 0)
	{
		dialogueVolume -= 1;
	}
	if (distancePlus < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed && dialogueVolume < 10)
	{
		dialogueVolume += 1;
	}
}

void MenuSceneLayer::ControlButton(GameObject::Sptr controlButton, glm::vec2 mouse)
{
	DeclareButton(controlButton, mouse);
	double distance = glm::distance(mouse, controlButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		controls = true;
		options = false;
		pressed = true;
	}
}

void MenuSceneLayer::GraphicButton(GameObject::Sptr graphicButton, glm::vec2 mouse)
{
	DeclareButton(graphicButton, mouse);
	double distance = glm::distance(mouse, graphicButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		graphics = true;
		options = false;
		pressed = true;
	}
}

void MenuSceneLayer::WarmColorCorrectionButton(GameObject::Sptr warmColorCorrectButton, glm::vec2 mouse)
{
	DeclareButton(warmColorCorrectButton, mouse, true);
	double distance = glm::distance(mouse, warmColorCorrectButton->Get<RectTransform>()->GetPosition());
	Application& app = Application::Get();
	PostProcessingLayer::Sptr layer = app.GetLayer<PostProcessingLayer>();

	if (distance < buttonDistance - 15 && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		warmColorCorrect = !warmColorCorrect;
	}

	if (warmColorCorrect)
	{
		layer->GetEffects()[1]->Enabled = warmColorCorrect;
		warmColorCorrectButton->Get<GuiText>()->SetText("Warm Color Correction\t\tON");
	}
	if (!warmColorCorrect)
	{
		layer->GetEffects()[1]->Enabled = warmColorCorrect;
		warmColorCorrectButton->Get<GuiText>()->SetText("Warm Color Correction\t\tOFF");
	}
}

void MenuSceneLayer::CoolColorCorrectionButton(GameObject::Sptr coolColorCorrectButton, glm::vec2 mouse)
{
	DeclareButton(coolColorCorrectButton, mouse, true);
	double distance = glm::distance(mouse, coolColorCorrectButton->Get<RectTransform>()->GetPosition());
	Application& app = Application::Get();
	PostProcessingLayer::Sptr layer = app.GetLayer<PostProcessingLayer>();

	if (distance < buttonDistance - 15 && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		coolColorCorrect = !coolColorCorrect;
	}

	if (coolColorCorrect)
	{
		layer->GetEffects()[0]->Enabled = coolColorCorrect;
		coolColorCorrectButton->Get<GuiText>()->SetText("Cool Color Correction\t\tON");
	}
	if (!coolColorCorrect)
	{
		layer->GetEffects()[0]->Enabled = coolColorCorrect;
		coolColorCorrectButton->Get<GuiText>()->SetText("Cool Color Correction\t\tOFF");
	}
}

void MenuSceneLayer::PixelButton(GameObject::Sptr pixelButton, glm::vec2 mouse)
{
	DeclareButton(pixelButton, mouse, true);
	double distance = glm::distance(mouse, pixelButton->Get<RectTransform>()->GetPosition());
	Application& app = Application::Get();
	PostProcessingLayer::Sptr layer = app.GetLayer<PostProcessingLayer>();

	if (distance < buttonDistance - 15 && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		pixelEffect = !pixelEffect;
	}

	if (pixelEffect)
	{
		layer->GetEffects()[3]->Enabled = pixelEffect;
		pixelButton->Get<GuiText>()->SetText("Pixel Effect\t\tON");
	}
	if (!pixelEffect)
	{
		layer->GetEffects()[3]->Enabled = pixelEffect;
		pixelButton->Get<GuiText>()->SetText("Pixel Effect\t\tOFF");
	}
}

void MenuSceneLayer::FilmGrainButton(GameObject::Sptr filmGrainButton, glm::vec2 mouse)
{
	DeclareButton(filmGrainButton, mouse, true);
	double distance = glm::distance(mouse, filmGrainButton->Get<RectTransform>()->GetPosition());
	Application& app = Application::Get();
	PostProcessingLayer::Sptr layer = app.GetLayer<PostProcessingLayer>();

	if (distance < buttonDistance - 15 && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		filmGrainEffect = !filmGrainEffect;
	}

	if (filmGrainEffect)
	{
		layer->GetEffects()[2]->Enabled = filmGrainEffect;
		filmGrainButton->Get<GuiText>()->SetText("Film Grain Effect\t\tON");
	}
	if (!filmGrainEffect)
	{
		layer->GetEffects()[2]->Enabled = filmGrainEffect;
		filmGrainButton->Get<GuiText>()->SetText("Film Grain Effect\t\tOFF");
	}
}

void MenuSceneLayer::TextureButton(GameObject::Sptr textureButton, glm::vec2 mouse)
{
	DeclareButton(textureButton, mouse, true);
	double distance = glm::distance(mouse, textureButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance - 15 && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		textureToggle = !textureToggle;
	}

	if (textureToggle)
	{
		textureButton->Get<GuiText>()->SetText("Texture\t\tON");
	}
	if (!textureToggle)
	{
		textureButton->Get<GuiText>()->SetText("Texture\t\tOFF");
	}
}

void MenuSceneLayer::ShadowMappingButton(GameObject::Sptr shadowMapButton, GameObject::Sptr shadowMap, glm::vec2 mouse)
{
	DeclareButton(shadowMapButton, mouse, true);
	double distance = glm::distance(mouse, shadowMapButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance - 15 && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		shadowMapping = !shadowMapping;
	}

	if (shadowMapping)
	{
		shadowMap->Get<ShadowCamera>()->Amount = 1.0f;
		shadowMapButton->Get<GuiText>()->SetText("Shadow Mapping\t\tON");
	}
	if (!shadowMapping)
	{
		shadowMap->Get<ShadowCamera>()->Amount = 0.0f;
		shadowMapButton->Get<GuiText>()->SetText("Shadow Mapping\t\tOFF");
	}
}

void MenuSceneLayer::LightingButton(GameObject::Sptr lightingButton, GameObject::Sptr shadowMap, glm::vec2 mouse)
{
	DeclareButton(lightingButton, mouse, true);
	double distance = glm::distance(mouse, lightingButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance - 15 && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		lightingToggle = !lightingToggle;
	}

	if (lightingToggle)
	{
		shadowMap->Get<ShadowCamera>()->IsEnabled = lightingToggle;
		lightingButton->Get<GuiText>()->SetText("Lighting\t\tON");
	}
	if (!lightingToggle)
	{
		shadowMap->Get<ShadowCamera>()->IsEnabled = lightingToggle;
		lightingButton->Get<GuiText>()->SetText("Lighting\t\tOFF");
	}
}

void MenuSceneLayer::_CreateScene()
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

		// Create meshes
		MeshResource::Sptr backstandMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Backstand.obj");
		MeshResource::Sptr flagMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Flag.obj");
		MeshResource::Sptr gatesMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Gates.obj");
		MeshResource::Sptr groundMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Ground.obj");
		MeshResource::Sptr polesMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Poles.obj");
		MeshResource::Sptr polesMesh2 = ResourceManager::CreateAsset<MeshResource>("models/arena/Poles2.obj");
		MeshResource::Sptr standsMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Stands.obj");
		MeshResource::Sptr standsMesh2 = ResourceManager::CreateAsset<MeshResource>("models/arena/Stands2.obj");
		MeshResource::Sptr wallsMesh = ResourceManager::CreateAsset<MeshResource>("models/arena/Walls.obj");
		MeshResource::Sptr bannerMesh = ResourceManager::CreateAsset<MeshResource>("models/animations/Banner/Banner_1.obj");

		// Create textures
		Texture2D::Sptr sand = ResourceManager::CreateAsset<Texture2D>("textures/arena/Sand.png");
		Texture2D::Sptr pole_tex = ResourceManager::CreateAsset<Texture2D>("textures/arena/Pole_Tex.png");
		Texture2D::Sptr gate_tex = ResourceManager::CreateAsset<Texture2D>("textures/arena/Gate_Tex.png");
		Texture2D::Sptr flag_tex = ResourceManager::CreateAsset<Texture2D>("textures/arena/Flag Tex.png");
		Texture2D::Sptr backstand_tex = ResourceManager::CreateAsset<Texture2D>("textures/arena/BackStand_Tex.png");
		Texture2D::Sptr sandstone_tex = ResourceManager::CreateAsset<Texture2D>("textures/arena/SandStone_Tex.png");
		Texture2D::Sptr wood_tex = ResourceManager::CreateAsset<Texture2D>("textures/arena/Wood_Tex.png");
		Texture2D::Sptr banner_tex = ResourceManager::CreateAsset<Texture2D>("textures/BannerTexture.png");

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

		// Create materials
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

		// Create camera
		GameObject::Sptr camera = scene->MainCamera->GetGameObject()->SelfRef();
		{
			camera->SetPostion(glm::vec3(20.0f, 17.0f, 5.0f));
			camera->LookAt(glm::vec3(0.0f));
			camera->SetRotation(glm::vec3(35.0f, 0.0f, -120.0f));

			RotatingBehaviour::Sptr behaviour = camera->Add<RotatingBehaviour>();
			behaviour->RotationSpeed = glm::vec3(0.0f, 0.0f, 20.0f);
		}

		// Create animation frames
		bannerWind.SetAnimation("Banner/Banner_", 125);
		bannerWind.SetTime(0.1f);

		// Create game objects
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
			ground->SetScale(glm::vec3(0.8f));

			RenderComponent::Sptr renderer = ground->Add<RenderComponent>();
			renderer->SetMesh(groundMesh);
			renderer->SetMaterial(sandMaterial);
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

		GameObject::Sptr menuPanel = scene->CreateGameObject("Menu Panel");
		{
			RectTransform::Sptr transform = menuPanel->Add<RectTransform>();
			transform->SetMin({ -12, -12 });
			transform->SetMax({ 2000, 2000 });

			GuiPanel::Sptr panel = menuPanel->Add<GuiPanel>();
			panel->SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			GameObject::Sptr logo = scene->CreateGameObject("Logo");
			{
				RectTransform::Sptr transform = logo->Add<RectTransform>();
				transform->SetMin({ 550, 130 });
				transform->SetMax({ 980, 370 });
				transform->SetPosition({ app.GetWindowSize().x / 2, 250 });

				GuiPanel::Sptr panel = logo->Add<GuiPanel>();
				panel->SetColor(glm::vec4(1.0f));

				panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("images/Slime Skirmish Logo.png"));
			}

			GameObject::Sptr startText = scene->CreateGameObject("Start Text");
			{
				RectTransform::Sptr transform = startText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x / 2, 450 });

				GuiPanel::Sptr panel = startText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = startText->Add<GuiText>();
				text->SetText("Start");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr optionsText = scene->CreateGameObject("Options Text");
			{
				RectTransform::Sptr transform = optionsText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x / 2, 550 });

				GuiPanel::Sptr panel = optionsText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = optionsText->Add<GuiText>();
				text->SetText("Options");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr exitText = scene->CreateGameObject("Exit Text");
			{
				RectTransform::Sptr transform = exitText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x / 2, 650 });

				GuiPanel::Sptr panel = exitText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = exitText->Add<GuiText>();
				text->SetText("Exit");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr backText = scene->CreateGameObject("Back Text");
			{
				RectTransform::Sptr transform = backText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 650 });

				GuiPanel::Sptr panel = backText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = backText->Add<GuiText>();
				text->SetText("Back");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr graphicText = scene->CreateGameObject("Graphic Text");
			{
				RectTransform::Sptr transform = graphicText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = graphicText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = graphicText->Add<GuiText>();
				text->SetText("Graphics");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr controlText = scene->CreateGameObject("Control Text");
			{
				RectTransform::Sptr transform = controlText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = controlText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = controlText->Add<GuiText>();
				text->SetText("Controls");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr soundText = scene->CreateGameObject("Sound Text");
			{
				RectTransform::Sptr transform = soundText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = soundText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = soundText->Add<GuiText>();
				text->SetText("Sounds");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr warmColorCorrectionText = scene->CreateGameObject("Warm Color Correction Text");
			{
				RectTransform::Sptr transform = warmColorCorrectionText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = warmColorCorrectionText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 20.0f);
				font->Bake();

				GuiText::Sptr text = warmColorCorrectionText->Add<GuiText>();
				text->SetText("Warm Color Correction\t\tOFF");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}
			GameObject::Sptr coolColorCorrectionText = scene->CreateGameObject("Cool Color Correction Text");
			{
				RectTransform::Sptr transform = coolColorCorrectionText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = coolColorCorrectionText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 20.0f);
				font->Bake();

				GuiText::Sptr text = coolColorCorrectionText->Add<GuiText>();
				text->SetText("Cool Color Correction\t\tOFF");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}
			GameObject::Sptr pixelationText = scene->CreateGameObject("Pixelation Text");
			{
				RectTransform::Sptr transform = pixelationText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = pixelationText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 20.0f);
				font->Bake();

				GuiText::Sptr text = pixelationText->Add<GuiText>();
				text->SetText("Pixel Effect\t\tOFF");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}
			GameObject::Sptr filmGrainText = scene->CreateGameObject("Film Grain Text");
			{
				RectTransform::Sptr transform = filmGrainText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = filmGrainText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 20.0f);
				font->Bake();

				GuiText::Sptr text = filmGrainText->Add<GuiText>();
				text->SetText("Film Grain Effect\t\tOFF");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}
			GameObject::Sptr textureText = scene->CreateGameObject("Texture Text");
			{
				RectTransform::Sptr transform = textureText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = textureText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 20.0f);
				font->Bake();

				GuiText::Sptr text = textureText->Add<GuiText>();
				text->SetText("Texture\t\tOFF");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}
			GameObject::Sptr shadowMappingText = scene->CreateGameObject("Shadow Mapping Text");
			{
				RectTransform::Sptr transform = shadowMappingText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = shadowMappingText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 20.0f);
				font->Bake();

				GuiText::Sptr text = shadowMappingText->Add<GuiText>();
				text->SetText("Shadow Mapping\t\tOFF");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}
			GameObject::Sptr lightingText = scene->CreateGameObject("Lighting Text");
			{
				RectTransform::Sptr transform = lightingText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = lightingText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 20.0f);
				font->Bake();

				GuiText::Sptr text = lightingText->Add<GuiText>();
				text->SetText("Lighting\t\tOFF");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr controlListText = scene->CreateGameObject("Control List Text");
			{
				RectTransform::Sptr transform = controlListText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = controlListText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = controlListText->Add<GuiText>();
				text->SetText("W - Move Up\n\n\nS - Move Down\n\n\nA - Move Left\n\n\nD - Move Right\n\n\nEsc - Pause");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr volumeText = scene->CreateGameObject("Volume Text");
			{
				RectTransform::Sptr transform = volumeText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = volumeText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = volumeText->Add<GuiText>();
				std::string volumeString = "Music Volume\t\t\t\t" + std::to_string(int(musicVolume));
				text->SetText(volumeString);
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}
			GameObject::Sptr volumeMinus = scene->CreateGameObject("Volume Minus");
			{
				RectTransform::Sptr transform = volumeMinus->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = volumeMinus->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 30.0f);
				font->Bake();

				GuiText::Sptr text = volumeMinus->Add<GuiText>();
				text->SetText("-");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}
			GameObject::Sptr volumePlus = scene->CreateGameObject("Volume Plus");
			{
				RectTransform::Sptr transform = volumePlus->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 550 });

				GuiPanel::Sptr panel = volumePlus->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 30.0f);
				font->Bake();

				GuiText::Sptr text = volumePlus->Add<GuiText>();
				text->SetText("+");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr effectVolumeText = scene->CreateGameObject("Effect Volume Text");
			{
				RectTransform::Sptr transform = effectVolumeText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 450 });

				GuiPanel::Sptr panel = effectVolumeText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = effectVolumeText->Add<GuiText>();
				std::string volumeString = "Sound Effect Volume\t\t\t\t" + std::to_string(int(dialogueVolume));
				text->SetText(volumeString);
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}
			GameObject::Sptr effectVolumeMinus = scene->CreateGameObject("Effect Volume Minus");
			{
				RectTransform::Sptr transform = effectVolumeMinus->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 450 });

				GuiPanel::Sptr panel = effectVolumeMinus->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 30.0f);
				font->Bake();

				GuiText::Sptr text = effectVolumeMinus->Add<GuiText>();
				text->SetText("-");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}
			GameObject::Sptr effectVolumePlus = scene->CreateGameObject("Effect Volume Plus");
			{
				RectTransform::Sptr transform = effectVolumePlus->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 450 });

				GuiPanel::Sptr panel = effectVolumePlus->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 30.0f);
				font->Bake();

				GuiText::Sptr text = effectVolumePlus->Add<GuiText>();
				text->SetText("+");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr dialogueVolumeText = scene->CreateGameObject("Dialogue Volume Text");
			{
				RectTransform::Sptr transform = dialogueVolumeText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 450 });

				GuiPanel::Sptr panel = dialogueVolumeText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = dialogueVolumeText->Add<GuiText>();
				std::string volumeString = "Sound Dialogue Volume\t\t\t\t" + std::to_string(0);
				text->SetText(volumeString);
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}
			GameObject::Sptr dialogueVolumeMinus = scene->CreateGameObject("Dialogue Volume Minus");
			{
				RectTransform::Sptr transform = dialogueVolumeMinus->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 450 });

				GuiPanel::Sptr panel = dialogueVolumeMinus->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 30.0f);
				font->Bake();

				GuiText::Sptr text = dialogueVolumeMinus->Add<GuiText>();
				text->SetText("-");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}
			GameObject::Sptr dialogueVolumePlus = scene->CreateGameObject("Dialogue Volume Plus");
			{
				RectTransform::Sptr transform = dialogueVolumePlus->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x * 2, 450 });

				GuiPanel::Sptr panel = dialogueVolumePlus->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 30.0f);
				font->Bake();

				GuiText::Sptr text = dialogueVolumePlus->Add<GuiText>();
				text->SetText("+");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			menuPanel->AddChild(logo);
			menuPanel->AddChild(startText);
			menuPanel->AddChild(optionsText);
			menuPanel->AddChild(exitText);

			menuPanel->AddChild(backText);

			menuPanel->AddChild(graphicText);
			menuPanel->AddChild(controlText);
			menuPanel->AddChild(soundText);

			menuPanel->AddChild(warmColorCorrectionText);
			menuPanel->AddChild(coolColorCorrectionText);
			menuPanel->AddChild(pixelationText);
			menuPanel->AddChild(filmGrainText);
			menuPanel->AddChild(textureText);
			menuPanel->AddChild(shadowMappingText);
			menuPanel->AddChild(lightingText);

			menuPanel->AddChild(controlListText);

			menuPanel->AddChild(volumeText);
			menuPanel->AddChild(volumeMinus);
			menuPanel->AddChild(volumePlus);

			menuPanel->AddChild(effectVolumeText);
			menuPanel->AddChild(effectVolumeMinus);
			menuPanel->AddChild(effectVolumePlus);

			menuPanel->AddChild(dialogueVolumeText);
			menuPanel->AddChild(dialogueVolumeMinus);
			menuPanel->AddChild(dialogueVolumePlus);
		}

		GameObject::Sptr shadowMap = scene->CreateGameObject("Shadow Map");
		{
			// Set position in the scene
			//shadowMap->SetPostion(glm::vec3(100.0f, 120.0f, 75.0f));
			shadowMap->SetPostion(glm::vec3(0.0f, 160.0f, 90.0f));
			shadowMap->LookAt(glm::vec3(20.0f, 15.0f, 0.0f));
			//shadowMap->SetRotation(glm::vec3(45, 0, 115));
			shadowMap->SetRotation(glm::vec3(-25.0f, -4.0f, 0.0f));
			//shadowMap->SetScale(glm::vec3(10.0f));

			// Create and attach a renderer for the monkey
			ShadowCamera::Sptr shadowCam = shadowMap->Add<ShadowCamera>();
			shadowCam->SetProjection(glm::perspective(glm::radians(120.0f), 1.0f, 0.1f, 200.0f));
			//shadowCam->SetProjection(glm::ortho(10.0f, -10.0f, 10.0f, -10.0f, 0.1f, 100.0f));
			shadowCam->Intensity = 1.5f;
		}

		GuiBatcher::SetDefaultTexture(ResourceManager::CreateAsset<Texture2D>("textures/ui-sprite.png"));
		GuiBatcher::SetDefaultBorderRadius(8);

		// Save the asset manifest for all the resources we just loaded
		ResourceManager::SaveManifest("menu-screen-manifest.json");

		// Save the scene to a JSON file
		scene->Save("scenes/menu-screen.json");

		// Send the scene to the application
		app.LoadScene(scene);
	}
}

void MenuSceneLayer::_MainLoop()
{
	Application& app = Application::Get();
	Scene::Sptr scene = app.CurrentScene();
	Timing& time = Timing::Current();

	time.SetTimeScale(1);

	GameObject::Sptr shadowMap = scene->FindObjectByName("Shadow Map");

	GameObject::Sptr logo = scene->FindObjectByName("Logo");
	GameObject::Sptr startText = scene->FindObjectByName("Start Text");
	GameObject::Sptr optionsText = scene->FindObjectByName("Options Text");
	GameObject::Sptr exitText = scene->FindObjectByName("Exit Text");
	
	GameObject::Sptr backText = scene->FindObjectByName("Back Text");
	
	GameObject::Sptr graphicText = scene->FindObjectByName("Graphic Text");
	GameObject::Sptr controlText = scene->FindObjectByName("Control Text");
	GameObject::Sptr soundText = scene->FindObjectByName("Sound Text");
	
	GameObject::Sptr warmColorCorrectionText = scene->FindObjectByName("Warm Color Correction Text");
	GameObject::Sptr coolColorCorrectionText = scene->FindObjectByName("Cool Color Correction Text");
	GameObject::Sptr pixelationText = scene->FindObjectByName("Pixelation Text");
	GameObject::Sptr filmGrainText = scene->FindObjectByName("Film Grain Text");
	GameObject::Sptr textureText = scene->FindObjectByName("Texture Text");
	GameObject::Sptr shadowMappingText = scene->FindObjectByName("Shadow Mapping Text");
	GameObject::Sptr lightingText = scene->FindObjectByName("Lighting Text");

	GameObject::Sptr controlListText = scene->FindObjectByName("Control List Text");

	GameObject::Sptr volumeText = scene->FindObjectByName("Volume Text");
	GameObject::Sptr volumeMinus = scene->FindObjectByName("Volume Minus");
	GameObject::Sptr volumePlus = scene->FindObjectByName("Volume Plus");

	GameObject::Sptr effectVolumeText = scene->FindObjectByName("Effect Volume Text");
	GameObject::Sptr effectVolumeMinus = scene->FindObjectByName("Effect Volume Minus");
	GameObject::Sptr effectVolumePlus = scene->FindObjectByName("Effect Volume Plus");

	GameObject::Sptr dialogueVolumeText = scene->FindObjectByName("Dialogue Volume Text");
	GameObject::Sptr dialogueVolumeMinus = scene->FindObjectByName("Dialogue Volume Minus");
	GameObject::Sptr dialogueVolumePlus = scene->FindObjectByName("Dialogue Volume Plus");

	float middle = app.GetWindowSize().x / 2;
	float right = app.GetWindowSize().x / 2 + app.GetWindowSize().x / 2 + app.GetWindowSize().x / 2;
	float left = app.GetWindowSize().x / 2 - app.GetWindowSize().x / 2 - app.GetWindowSize().x / 2;

	float middleY = app.GetWindowSize().y / 2 + 50;

	float difference = 100.0f;
	float volumeDifferenceMinus = 50.0f;
	float volumeDifferencePlus = 125.0f;

	glm::vec2 mousePosition = glm::vec2(InputEngine::GetMousePos().x, InputEngine::GetMousePos().y);

	StartButton(startText, mousePosition);
	OptionsButton(optionsText, mousePosition);
	ExitButton(exitText, mousePosition);

	BackButton(backText, mousePosition);

	GraphicButton(graphicText, mousePosition);
	ControlButton(controlText, mousePosition);
	SoundButton(soundText, mousePosition);

	CoolColorCorrectionButton(coolColorCorrectionText, mousePosition);
	WarmColorCorrectionButton(warmColorCorrectionText, mousePosition);
	PixelButton(pixelationText, mousePosition);
	FilmGrainButton(filmGrainText, mousePosition);
	TextureButton(textureText, mousePosition);
	ShadowMappingButton(shadowMappingText, shadowMap, mousePosition);
	LightingButton(lightingText, shadowMap, mousePosition);

	VolumeButtons(volumeText, volumeMinus, volumePlus, mousePosition);
	EffectVolumeButtons(effectVolumeText, effectVolumeMinus, effectVolumePlus, mousePosition);
	DialogueVolumeButtons(dialogueVolumeText, dialogueVolumeMinus, dialogueVolumePlus, mousePosition);

	std::vector<GameObject::Sptr> banners;
	{
		banners.resize(5);
		for (size_t i = 0; i < 5; i++)
		{
			banners[i] = scene->FindObjectByName("Banner" + std::to_string(i));
			bannerWind.UpdateAnimation(banners[i], time.DeltaTime());
		}
	}

	std::string volumeString = "Music Volume\t\t\t" + std::to_string(int(musicVolume));
	volumeText->Get<GuiText>()->SetText(volumeString);

	std::string effectVolumeString = "Effect Volume\t\t\t" + std::to_string(int(effectVolume));
	effectVolumeText->Get<GuiText>()->SetText(effectVolumeString);

	std::string dialogueVolumeString = "Dialogue Volume\t\t\t" + std::to_string(int(dialogueVolume));
	dialogueVolumeText->Get<GuiText>()->SetText(dialogueVolumeString);

	// Options pressed
	if (pressed && options && !sounds && !graphics && !controls)
	{
		float t = 0.1;

		if (t < 1.f) 
		{ 
			t += time.DeltaTime(); 

			logo->Get<RectTransform>()->SetPosition(LERP(logo->Get<RectTransform>()->GetPosition(), { middle, middleY - 400 }, t));

			startText->Get<RectTransform>()->SetPosition(LERP(startText->Get<RectTransform>()->GetPosition(), { left, middleY - difference }, t));
			optionsText->Get<RectTransform>()->SetPosition(LERP(optionsText->Get<RectTransform>()->GetPosition(), { left, middleY }, t));
			exitText->Get<RectTransform>()->SetPosition(LERP(exitText->Get<RectTransform>()->GetPosition(), { left, middleY + difference }, t));

			soundText->Get<RectTransform>()->SetPosition(LERP(soundText->Get<RectTransform>()->GetPosition(), { middle, middleY }, t));
			graphicText->Get<RectTransform>()->SetPosition(LERP(graphicText->Get<RectTransform>()->GetPosition(), { middle, middleY - difference * 2 }, t));
			controlText->Get<RectTransform>()->SetPosition(LERP(controlText->Get<RectTransform>()->GetPosition(), { middle, middleY - difference }, t));

			pixelationText->Get<RectTransform>()->SetPosition(LERP(pixelationText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 3 }, t));
			filmGrainText->Get<RectTransform>()->SetPosition(LERP(filmGrainText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2.5 }, t));
			coolColorCorrectionText->Get<RectTransform>()->SetPosition(LERP(coolColorCorrectionText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2 }, t));
			warmColorCorrectionText->Get<RectTransform>()->SetPosition(LERP(warmColorCorrectionText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 1.5 }, t));
			textureText->Get<RectTransform>()->SetPosition(LERP(textureText->Get<RectTransform>()->GetPosition(), { right, middleY - difference }, t));
			shadowMappingText->Get<RectTransform>()->SetPosition(LERP(shadowMappingText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 0.5 }, t));
			lightingText->Get<RectTransform>()->SetPosition(LERP(lightingText->Get<RectTransform>()->GetPosition(), { right, middleY }, t));

			controlListText->Get<RectTransform>()->SetPosition(LERP(controlListText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2.5 }, t));

			volumeText->Get<RectTransform>()->SetPosition(LERP(volumeText->Get<RectTransform>()->GetPosition(), { right, middleY }, t));
			volumeMinus->Get<RectTransform>()->SetPosition(LERP(volumeMinus->Get<RectTransform>()->GetPosition(), { right + volumeDifferenceMinus, middleY }, t));
			volumePlus->Get<RectTransform>()->SetPosition(LERP(volumePlus->Get<RectTransform>()->GetPosition(), { right + volumeDifferencePlus, middleY }, t));

			effectVolumeText->Get<RectTransform>()->SetPosition(LERP(effectVolumeText->Get<RectTransform>()->GetPosition(), { right, middleY - difference }, t));
			effectVolumeMinus->Get<RectTransform>()->SetPosition(LERP(effectVolumeMinus->Get<RectTransform>()->GetPosition(), { right + volumeDifferenceMinus, middleY - difference }, t));
			effectVolumePlus->Get<RectTransform>()->SetPosition(LERP(effectVolumePlus->Get<RectTransform>()->GetPosition(), { right + volumeDifferencePlus, middleY - difference }, t));

			dialogueVolumeText->Get<RectTransform>()->SetPosition(LERP(dialogueVolumeText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2 }, t));
			dialogueVolumeMinus->Get<RectTransform>()->SetPosition(LERP(dialogueVolumeMinus->Get<RectTransform>()->GetPosition(), { right + volumeDifferenceMinus + 10, middleY - difference * 2 }, t));
			dialogueVolumePlus->Get<RectTransform>()->SetPosition(LERP(dialogueVolumePlus->Get<RectTransform>()->GetPosition(), { right + volumeDifferencePlus + 10, middleY - difference * 2 }, t));

			backText->Get<RectTransform>()->SetPosition(LERP(backText->Get<RectTransform>()->GetPosition(), { middle, middleY + difference }, t));
		}
		else pressed = false;
	}
	// Back to menu pressed
	if (pressed && !options && !sounds && !graphics && !controls)
	{
		float t = 0.1f;

		if (t < 1.f)
		{
			t += time.DeltaTime();

			logo->Get<RectTransform>()->SetPosition(LERP(logo->Get<RectTransform>()->GetPosition(), { middle, middleY - 400 }, t));

			startText->Get<RectTransform>()->SetPosition(LERP(startText->Get<RectTransform>()->GetPosition(), { middle, middleY - difference }, t));
			optionsText->Get<RectTransform>()->SetPosition(LERP(optionsText->Get<RectTransform>()->GetPosition(), { middle, middleY }, t));
			exitText->Get<RectTransform>()->SetPosition(LERP(exitText->Get<RectTransform>()->GetPosition(), { middle, middleY + difference }, t));

			soundText->Get<RectTransform>()->SetPosition(LERP(soundText->Get<RectTransform>()->GetPosition(), { right, middleY }, t));
			graphicText->Get<RectTransform>()->SetPosition(LERP(graphicText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2 }, t));
			controlText->Get<RectTransform>()->SetPosition(LERP(controlText->Get<RectTransform>()->GetPosition(), { right, middleY - difference }, t));

			pixelationText->Get<RectTransform>()->SetPosition(LERP(pixelationText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 3 }, t));
			filmGrainText->Get<RectTransform>()->SetPosition(LERP(filmGrainText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2.5 }, t));
			coolColorCorrectionText->Get<RectTransform>()->SetPosition(LERP(coolColorCorrectionText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2 }, t));
			warmColorCorrectionText->Get<RectTransform>()->SetPosition(LERP(warmColorCorrectionText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 1.5 }, t));
			textureText->Get<RectTransform>()->SetPosition(LERP(textureText->Get<RectTransform>()->GetPosition(), { right, middleY - difference }, t));
			shadowMappingText->Get<RectTransform>()->SetPosition(LERP(shadowMappingText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 0.5 }, t));
			lightingText->Get<RectTransform>()->SetPosition(LERP(lightingText->Get<RectTransform>()->GetPosition(), { right, middleY }, t));

			controlListText->Get<RectTransform>()->SetPosition(LERP(controlListText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2.5 }, t));

			volumeText->Get<RectTransform>()->SetPosition(LERP(volumeText->Get<RectTransform>()->GetPosition(), { right, middleY }, t));
			volumeMinus->Get<RectTransform>()->SetPosition(LERP(volumeMinus->Get<RectTransform>()->GetPosition(), { right + volumeDifferenceMinus, middleY }, t));
			volumePlus->Get<RectTransform>()->SetPosition(LERP(volumePlus->Get<RectTransform>()->GetPosition(), { right + volumeDifferencePlus, middleY }, t));

			effectVolumeText->Get<RectTransform>()->SetPosition(LERP(effectVolumeText->Get<RectTransform>()->GetPosition(), { right, middleY - difference }, t));
			effectVolumeMinus->Get<RectTransform>()->SetPosition(LERP(effectVolumeMinus->Get<RectTransform>()->GetPosition(), { right + volumeDifferenceMinus, middleY - difference }, t));
			effectVolumePlus->Get<RectTransform>()->SetPosition(LERP(effectVolumePlus->Get<RectTransform>()->GetPosition(), { right + volumeDifferencePlus, middleY - difference }, t));

			dialogueVolumeText->Get<RectTransform>()->SetPosition(LERP(dialogueVolumeText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2 }, t));
			dialogueVolumeMinus->Get<RectTransform>()->SetPosition(LERP(dialogueVolumeMinus->Get<RectTransform>()->GetPosition(), { right + volumeDifferenceMinus + 10, middleY - difference * 2 }, t));
			dialogueVolumePlus->Get<RectTransform>()->SetPosition(LERP(dialogueVolumePlus->Get<RectTransform>()->GetPosition(), { right + volumeDifferencePlus + 10, middleY - difference * 2 }, t));

			backText->Get<RectTransform>()->SetPosition(LERP(backText->Get<RectTransform>()->GetPosition(), { right, middleY + difference }, t));
		}
		else pressed = false;
	}
	// Sounds pressed
	if (pressed && !options && sounds && !graphics && !controls)
	{
		float t = 0.1;

		if (t < 1.f)
		{
			t += time.DeltaTime();

			logo->Get<RectTransform>()->SetPosition(LERP(logo->Get<RectTransform>()->GetPosition(), { middle, middleY - 400 }, t));

			startText->Get<RectTransform>()->SetPosition(LERP(startText->Get<RectTransform>()->GetPosition(), { left, middleY - difference }, t));
			optionsText->Get<RectTransform>()->SetPosition(LERP(optionsText->Get<RectTransform>()->GetPosition(), { left, middleY }, t));
			exitText->Get<RectTransform>()->SetPosition(LERP(exitText->Get<RectTransform>()->GetPosition(), { left, middleY + difference }, t));

			soundText->Get<RectTransform>()->SetPosition(LERP(soundText->Get<RectTransform>()->GetPosition(), { left, middleY }, t));
			graphicText->Get<RectTransform>()->SetPosition(LERP(graphicText->Get<RectTransform>()->GetPosition(), { left, middleY - difference * 2 }, t));
			controlText->Get<RectTransform>()->SetPosition(LERP(controlText->Get<RectTransform>()->GetPosition(), { left, middleY - difference }, t));

			pixelationText->Get<RectTransform>()->SetPosition(LERP(pixelationText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 3 }, t));
			filmGrainText->Get<RectTransform>()->SetPosition(LERP(filmGrainText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2.5 }, t));
			coolColorCorrectionText->Get<RectTransform>()->SetPosition(LERP(coolColorCorrectionText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2 }, t));
			warmColorCorrectionText->Get<RectTransform>()->SetPosition(LERP(warmColorCorrectionText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 1.5 }, t));
			textureText->Get<RectTransform>()->SetPosition(LERP(textureText->Get<RectTransform>()->GetPosition(), { right, middleY - difference }, t));
			shadowMappingText->Get<RectTransform>()->SetPosition(LERP(shadowMappingText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 0.5 }, t));
			lightingText->Get<RectTransform>()->SetPosition(LERP(lightingText->Get<RectTransform>()->GetPosition(), { right, middleY }, t));

			controlListText->Get<RectTransform>()->SetPosition(LERP(controlListText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2.5 }, t));

			volumeText->Get<RectTransform>()->SetPosition(LERP(volumeText->Get<RectTransform>()->GetPosition(), { middle, middleY }, t));
			volumeMinus->Get<RectTransform>()->SetPosition(LERP(volumeMinus->Get<RectTransform>()->GetPosition(), { middle + volumeDifferenceMinus, middleY }, t));
			volumePlus->Get<RectTransform>()->SetPosition(LERP(volumePlus->Get<RectTransform>()->GetPosition(), { middle + volumeDifferencePlus, middleY }, t));

			effectVolumeText->Get<RectTransform>()->SetPosition(LERP(effectVolumeText->Get<RectTransform>()->GetPosition(), { middle, middleY - difference }, t));
			effectVolumeMinus->Get<RectTransform>()->SetPosition(LERP(effectVolumeMinus->Get<RectTransform>()->GetPosition(), { middle + volumeDifferenceMinus, middleY - difference }, t));
			effectVolumePlus->Get<RectTransform>()->SetPosition(LERP(effectVolumePlus->Get<RectTransform>()->GetPosition(), { middle + volumeDifferencePlus, middleY - difference }, t));

			dialogueVolumeText->Get<RectTransform>()->SetPosition(LERP(dialogueVolumeText->Get<RectTransform>()->GetPosition(), { middle, middleY - difference * 2 }, t));
			dialogueVolumeMinus->Get<RectTransform>()->SetPosition(LERP(dialogueVolumeMinus->Get<RectTransform>()->GetPosition(), { middle + volumeDifferenceMinus + 10, middleY - difference * 2 }, t));
			dialogueVolumePlus->Get<RectTransform>()->SetPosition(LERP(dialogueVolumePlus->Get<RectTransform>()->GetPosition(), { middle + volumeDifferencePlus + 10, middleY - difference * 2 }, t));

			backText->Get<RectTransform>()->SetPosition(LERP(backText->Get<RectTransform>()->GetPosition(), { middle, middleY + difference }, t));
		}
		else pressed = false;
	}
	// Graphics pressed
	if (pressed && !options && !sounds && graphics && !controls)
	{
		float t = 0.1;

		if (t < 1.f)
		{
			t += time.DeltaTime();

			logo->Get<RectTransform>()->SetPosition(LERP(logo->Get<RectTransform>()->GetPosition(), { middle, middleY - 800 }, t));

			startText->Get<RectTransform>()->SetPosition(LERP(startText->Get<RectTransform>()->GetPosition(), { left, middleY - difference }, t));
			optionsText->Get<RectTransform>()->SetPosition(LERP(optionsText->Get<RectTransform>()->GetPosition(), { left, middleY }, t));
			exitText->Get<RectTransform>()->SetPosition(LERP(exitText->Get<RectTransform>()->GetPosition(), { left, middleY + difference }, t));

			soundText->Get<RectTransform>()->SetPosition(LERP(soundText->Get<RectTransform>()->GetPosition(), { left, middleY }, t));
			graphicText->Get<RectTransform>()->SetPosition(LERP(graphicText->Get<RectTransform>()->GetPosition(), { left, middleY - difference * 2 }, t));
			controlText->Get<RectTransform>()->SetPosition(LERP(controlText->Get<RectTransform>()->GetPosition(), { left, middleY - difference }, t));

			pixelationText->Get<RectTransform>()->SetPosition(LERP(pixelationText->Get<RectTransform>()->GetPosition(), { middle, middleY - difference * 3 }, t));
			filmGrainText->Get<RectTransform>()->SetPosition(LERP(filmGrainText->Get<RectTransform>()->GetPosition(), { middle, middleY - difference * 2.5 }, t));
			coolColorCorrectionText->Get<RectTransform>()->SetPosition(LERP(coolColorCorrectionText->Get<RectTransform>()->GetPosition(), { middle, middleY - difference * 2 }, t));
			warmColorCorrectionText->Get<RectTransform>()->SetPosition(LERP(warmColorCorrectionText->Get<RectTransform>()->GetPosition(), { middle, middleY - difference * 1.5 }, t));
			textureText->Get<RectTransform>()->SetPosition(LERP(textureText->Get<RectTransform>()->GetPosition(), { middle, middleY - difference }, t));
			shadowMappingText->Get<RectTransform>()->SetPosition(LERP(shadowMappingText->Get<RectTransform>()->GetPosition(), { middle, middleY - difference * 0.5 }, t));
			lightingText->Get<RectTransform>()->SetPosition(LERP(lightingText->Get<RectTransform>()->GetPosition(), { middle, middleY }, t));

			controlListText->Get<RectTransform>()->SetPosition(LERP(controlListText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2.5 }, t));

			volumeText->Get<RectTransform>()->SetPosition(LERP(volumeText->Get<RectTransform>()->GetPosition(), { right, middleY }, t));
			volumeMinus->Get<RectTransform>()->SetPosition(LERP(volumeMinus->Get<RectTransform>()->GetPosition(), { right + volumeDifferenceMinus, middleY }, t));
			volumePlus->Get<RectTransform>()->SetPosition(LERP(volumePlus->Get<RectTransform>()->GetPosition(), { right + volumeDifferencePlus, middleY }, t));

			effectVolumeText->Get<RectTransform>()->SetPosition(LERP(effectVolumeText->Get<RectTransform>()->GetPosition(), { right, middleY - difference }, t));
			effectVolumeMinus->Get<RectTransform>()->SetPosition(LERP(effectVolumeMinus->Get<RectTransform>()->GetPosition(), { right + volumeDifferenceMinus, middleY - difference }, t));
			effectVolumePlus->Get<RectTransform>()->SetPosition(LERP(effectVolumePlus->Get<RectTransform>()->GetPosition(), { right + volumeDifferencePlus, middleY - difference }, t));

			dialogueVolumeText->Get<RectTransform>()->SetPosition(LERP(dialogueVolumeText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2 }, t));
			dialogueVolumeMinus->Get<RectTransform>()->SetPosition(LERP(dialogueVolumeMinus->Get<RectTransform>()->GetPosition(), { right + volumeDifferenceMinus + 10, middleY - difference * 2 }, t));
			dialogueVolumePlus->Get<RectTransform>()->SetPosition(LERP(dialogueVolumePlus->Get<RectTransform>()->GetPosition(), { right + volumeDifferencePlus + 10, middleY - difference * 2 }, t));

			backText->Get<RectTransform>()->SetPosition(LERP(backText->Get<RectTransform>()->GetPosition(), { middle, middleY + difference }, t));
		}
		else pressed = false;
	}
	// Controls pressed
	if (pressed && !options && !sounds && !graphics && controls)
	{
		float t = 0.1;

		if (t < 1.f)
		{
			t += time.DeltaTime();

			logo->Get<RectTransform>()->SetPosition(LERP(logo->Get<RectTransform>()->GetPosition(), { middle, middleY - 800 }, t));

			startText->Get<RectTransform>()->SetPosition(LERP(startText->Get<RectTransform>()->GetPosition(), { left, middleY - difference }, t));
			optionsText->Get<RectTransform>()->SetPosition(LERP(optionsText->Get<RectTransform>()->GetPosition(), { left, middleY }, t));
			exitText->Get<RectTransform>()->SetPosition(LERP(exitText->Get<RectTransform>()->GetPosition(), { left, middleY + difference }, t));

			soundText->Get<RectTransform>()->SetPosition(LERP(soundText->Get<RectTransform>()->GetPosition(), { left, middleY }, t));
			graphicText->Get<RectTransform>()->SetPosition(LERP(graphicText->Get<RectTransform>()->GetPosition(), { left, middleY - difference * 2 }, t));
			controlText->Get<RectTransform>()->SetPosition(LERP(controlText->Get<RectTransform>()->GetPosition(), { left, middleY - difference }, t));

			pixelationText->Get<RectTransform>()->SetPosition(LERP(pixelationText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 3 }, t));
			filmGrainText->Get<RectTransform>()->SetPosition(LERP(filmGrainText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2.5 }, t));
			coolColorCorrectionText->Get<RectTransform>()->SetPosition(LERP(coolColorCorrectionText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2 }, t));
			warmColorCorrectionText->Get<RectTransform>()->SetPosition(LERP(warmColorCorrectionText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 1.5 }, t));
			textureText->Get<RectTransform>()->SetPosition(LERP(textureText->Get<RectTransform>()->GetPosition(), { right, middleY - difference }, t));
			shadowMappingText->Get<RectTransform>()->SetPosition(LERP(shadowMappingText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 0.5 }, t));
			lightingText->Get<RectTransform>()->SetPosition(LERP(lightingText->Get<RectTransform>()->GetPosition(), { right, middleY }, t));

			controlListText->Get<RectTransform>()->SetPosition(LERP(controlListText->Get<RectTransform>()->GetPosition(), { middle, middleY - difference * 2.5 }, t));

			volumeText->Get<RectTransform>()->SetPosition(LERP(volumeText->Get<RectTransform>()->GetPosition(), { right, middleY }, t));
			volumeMinus->Get<RectTransform>()->SetPosition(LERP(volumeMinus->Get<RectTransform>()->GetPosition(), { right + volumeDifferenceMinus, middleY }, t));
			volumePlus->Get<RectTransform>()->SetPosition(LERP(volumePlus->Get<RectTransform>()->GetPosition(), { right + volumeDifferencePlus, middleY }, t));

			effectVolumeText->Get<RectTransform>()->SetPosition(LERP(effectVolumeText->Get<RectTransform>()->GetPosition(), { right, middleY - difference }, t));
			effectVolumeMinus->Get<RectTransform>()->SetPosition(LERP(effectVolumeMinus->Get<RectTransform>()->GetPosition(), { right + volumeDifferenceMinus, middleY - difference }, t));
			effectVolumePlus->Get<RectTransform>()->SetPosition(LERP(effectVolumePlus->Get<RectTransform>()->GetPosition(), { right + volumeDifferencePlus, middleY - difference }, t));

			dialogueVolumeText->Get<RectTransform>()->SetPosition(LERP(dialogueVolumeText->Get<RectTransform>()->GetPosition(), { right, middleY - difference * 2 }, t));
			dialogueVolumeMinus->Get<RectTransform>()->SetPosition(LERP(dialogueVolumeMinus->Get<RectTransform>()->GetPosition(), { right + volumeDifferenceMinus + 10, middleY - difference * 2 }, t));
			dialogueVolumePlus->Get<RectTransform>()->SetPosition(LERP(dialogueVolumePlus->Get<RectTransform>()->GetPosition(), { right + volumeDifferencePlus + 10, middleY - difference * 2 }, t));

			backText->Get<RectTransform>()->SetPosition(LERP(backText->Get<RectTransform>()->GetPosition(), { middle, middleY + difference }, t));
		}
		else pressed = false;
	}

	// While main menu
	if (!options && !pressed && !sounds && !graphics && !controls)
	{
		logo->Get<RectTransform>()->SetPosition({ middle, middleY - 400 });

		startText->Get<RectTransform>()->SetPosition({ middle, middleY - difference });
		optionsText->Get<RectTransform>()->SetPosition({ middle, middleY });
		exitText->Get<RectTransform>()->SetPosition({ middle, middleY + difference });

		soundText->Get<RectTransform>()->SetPosition({ right, middleY });
		graphicText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2 });
		controlText->Get<RectTransform>()->SetPosition({ right, middleY - difference });

		filmGrainText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 3 });
		pixelationText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2.5 });
		coolColorCorrectionText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2 });
		warmColorCorrectionText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 1.5 });
		textureText->Get<RectTransform>()->SetPosition({ right, middleY - difference });
		shadowMappingText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 0.5 });
		lightingText->Get<RectTransform>()->SetPosition({ right, middleY });

		controlListText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2.5 });

		volumeText->Get<RectTransform>()->SetPosition({ right, middleY });
		volumeMinus->Get<RectTransform>()->SetPosition({ right + volumeDifferenceMinus, middleY });
		volumePlus->Get<RectTransform>()->SetPosition({ right + volumeDifferencePlus, middleY });

		effectVolumeText->Get<RectTransform>()->SetPosition({ right, middleY - difference });
		effectVolumeMinus->Get<RectTransform>()->SetPosition({ right + volumeDifferenceMinus, middleY - difference });
		effectVolumePlus->Get<RectTransform>()->SetPosition({ right + volumeDifferencePlus, middleY - difference });

		dialogueVolumeText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2 });
		dialogueVolumeMinus->Get<RectTransform>()->SetPosition({ right + volumeDifferenceMinus + 10, middleY - difference * 2 });
		dialogueVolumePlus->Get<RectTransform>()->SetPosition({ right + volumeDifferencePlus + 10, middleY - difference * 2 });

		backText->Get<RectTransform>()->SetPosition({ right, middleY + difference });
	}
	// While options
	if (options && !pressed && !sounds && !graphics && !controls)
	{
		logo->Get<RectTransform>()->SetPosition({ middle, middleY - 400 });

		startText->Get<RectTransform>()->SetPosition({ left, middleY - difference });
		optionsText->Get<RectTransform>()->SetPosition({ left, middleY });
		exitText->Get<RectTransform>()->SetPosition({ left, middleY + difference });

		soundText->Get<RectTransform>()->SetPosition({ middle, middleY });
		graphicText->Get<RectTransform>()->SetPosition({ middle, middleY - difference * 2 });
		controlText->Get<RectTransform>()->SetPosition({ middle, middleY - difference });

		filmGrainText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 3 });
		pixelationText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2.5 });
		coolColorCorrectionText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2 });
		warmColorCorrectionText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 1.5 });
		textureText->Get<RectTransform>()->SetPosition({ right, middleY - difference });
		shadowMappingText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 0.5 });
		lightingText->Get<RectTransform>()->SetPosition({ right, middleY });

		controlListText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2.5 });

		volumeText->Get<RectTransform>()->SetPosition({ right, middleY });
		volumeMinus->Get<RectTransform>()->SetPosition({ right + volumeDifferenceMinus, middleY });
		volumePlus->Get<RectTransform>()->SetPosition({ right + volumeDifferencePlus, middleY });

		effectVolumeText->Get<RectTransform>()->SetPosition({ right, middleY - difference });
		effectVolumeMinus->Get<RectTransform>()->SetPosition({ right + volumeDifferenceMinus, middleY - difference });
		effectVolumePlus->Get<RectTransform>()->SetPosition({ right + volumeDifferencePlus, middleY - difference });

		dialogueVolumeText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2 });
		dialogueVolumeMinus->Get<RectTransform>()->SetPosition({ right + volumeDifferenceMinus + 10, middleY - difference * 2 });
		dialogueVolumePlus->Get<RectTransform>()->SetPosition({ right + volumeDifferencePlus + 10, middleY - difference * 2 });

		backText->Get<RectTransform>()->SetPosition({ middle, middleY + difference });
	}
	// While sounds
	if (!options && !pressed && sounds && !graphics && !controls)
	{
		logo->Get<RectTransform>()->SetPosition({ middle, middleY - 400 });

		startText->Get<RectTransform>()->SetPosition({ left, middleY - difference });
		optionsText->Get<RectTransform>()->SetPosition({ left, middleY });
		exitText->Get<RectTransform>()->SetPosition({ left, middleY + difference });

		soundText->Get<RectTransform>()->SetPosition({ left, middleY });
		graphicText->Get<RectTransform>()->SetPosition({ left, middleY - difference * 2 });
		controlText->Get<RectTransform>()->SetPosition({ left, middleY - difference });

		filmGrainText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 3 });
		pixelationText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2.5 });
		coolColorCorrectionText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2 });
		warmColorCorrectionText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 1.5 });
		textureText->Get<RectTransform>()->SetPosition({ right, middleY - difference });
		shadowMappingText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 0.5 });
		lightingText->Get<RectTransform>()->SetPosition({ right, middleY });

		controlListText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2.5 });

		volumeText->Get<RectTransform>()->SetPosition({ middle, middleY });
		volumeMinus->Get<RectTransform>()->SetPosition({ middle + volumeDifferenceMinus, middleY });
		volumePlus->Get<RectTransform>()->SetPosition({ middle + volumeDifferencePlus, middleY });

		effectVolumeText->Get<RectTransform>()->SetPosition({ middle, middleY - difference });
		effectVolumeMinus->Get<RectTransform>()->SetPosition({ middle + volumeDifferenceMinus, middleY - difference });
		effectVolumePlus->Get<RectTransform>()->SetPosition({ middle + volumeDifferencePlus, middleY - difference });

		dialogueVolumeText->Get<RectTransform>()->SetPosition({ middle, middleY - difference * 2 });
		dialogueVolumeMinus->Get<RectTransform>()->SetPosition({ middle + volumeDifferenceMinus + 10, middleY - difference * 2 });
		dialogueVolumePlus->Get<RectTransform>()->SetPosition({ middle + volumeDifferencePlus + 10, middleY - difference * 2 });

		backText->Get<RectTransform>()->SetPosition({ middle, middleY + difference });
	}
	// While graphics
	if (!options && !pressed && !sounds && graphics && !controls)
	{
		logo->Get<RectTransform>()->SetPosition({ middle, middleY - 800 });

		startText->Get<RectTransform>()->SetPosition({ left, middleY - difference });
		optionsText->Get<RectTransform>()->SetPosition({ left, middleY });
		exitText->Get<RectTransform>()->SetPosition({ left, middleY + difference });

		soundText->Get<RectTransform>()->SetPosition({ left, middleY });
		graphicText->Get<RectTransform>()->SetPosition({ left, middleY - difference * 2 });
		controlText->Get<RectTransform>()->SetPosition({ left, middleY - difference });

		filmGrainText->Get<RectTransform>()->SetPosition({ middle, middleY - difference * 3 });
		pixelationText->Get<RectTransform>()->SetPosition({ middle, middleY - difference * 2.5 });
		coolColorCorrectionText->Get<RectTransform>()->SetPosition({ middle, middleY - difference * 2 });
		warmColorCorrectionText->Get<RectTransform>()->SetPosition({ middle, middleY - difference * 1.5 });
		textureText->Get<RectTransform>()->SetPosition({ middle, middleY - difference });
		shadowMappingText->Get<RectTransform>()->SetPosition({ middle, middleY - difference * 0.5 });
		lightingText->Get<RectTransform>()->SetPosition({ middle, middleY });

		controlListText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2.5 });

		volumeText->Get<RectTransform>()->SetPosition({ right, middleY });
		volumeMinus->Get<RectTransform>()->SetPosition({ right + volumeDifferenceMinus, middleY });
		volumePlus->Get<RectTransform>()->SetPosition({ right + volumeDifferencePlus, middleY });

		effectVolumeText->Get<RectTransform>()->SetPosition({ right, middleY - difference });
		effectVolumeMinus->Get<RectTransform>()->SetPosition({ right + volumeDifferenceMinus, middleY - difference });
		effectVolumePlus->Get<RectTransform>()->SetPosition({ right + volumeDifferencePlus, middleY - difference });

		dialogueVolumeText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2 });
		dialogueVolumeMinus->Get<RectTransform>()->SetPosition({ right + volumeDifferenceMinus + 10, middleY - difference * 2 });
		dialogueVolumePlus->Get<RectTransform>()->SetPosition({ right + volumeDifferencePlus + 10, middleY - difference * 2 });

		backText->Get<RectTransform>()->SetPosition({ middle, middleY + difference });
	}
	// While controls
	if (!options && !pressed && !sounds && !graphics && controls)
	{
		logo->Get<RectTransform>()->SetPosition({ middle, middleY - 800 });

		startText->Get<RectTransform>()->SetPosition({ left, middleY - difference });
		optionsText->Get<RectTransform>()->SetPosition({ left, middleY });
		exitText->Get<RectTransform>()->SetPosition({ left, middleY + difference });

		soundText->Get<RectTransform>()->SetPosition({ left, middleY });
		graphicText->Get<RectTransform>()->SetPosition({ left, middleY - difference * 2 });
		controlText->Get<RectTransform>()->SetPosition({ left, middleY - difference });

		filmGrainText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 3 });
		pixelationText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2.5 });
		coolColorCorrectionText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2 });
		warmColorCorrectionText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 1.5 });
		textureText->Get<RectTransform>()->SetPosition({ right, middleY - difference });
		shadowMappingText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 0.5 });
		lightingText->Get<RectTransform>()->SetPosition({ right, middleY });

		controlListText->Get<RectTransform>()->SetPosition({ middle, middleY - difference * 2.5 });

		volumeText->Get<RectTransform>()->SetPosition({ right, middleY });
		volumeMinus->Get<RectTransform>()->SetPosition({ right + volumeDifferenceMinus, middleY });
		volumePlus->Get<RectTransform>()->SetPosition({ right + volumeDifferencePlus, middleY });

		effectVolumeText->Get<RectTransform>()->SetPosition({ right, middleY - difference });
		effectVolumeMinus->Get<RectTransform>()->SetPosition({ right + volumeDifferenceMinus, middleY - difference });
		effectVolumePlus->Get<RectTransform>()->SetPosition({ right + volumeDifferencePlus, middleY - difference });

		dialogueVolumeText->Get<RectTransform>()->SetPosition({ right, middleY - difference * 2 });
		dialogueVolumeMinus->Get<RectTransform>()->SetPosition({ right + volumeDifferenceMinus + 10, middleY - difference * 2 });
		dialogueVolumePlus->Get<RectTransform>()->SetPosition({ right + volumeDifferencePlus + 10, middleY - difference * 2 });

		backText->Get<RectTransform>()->SetPosition({ middle, middleY + difference });
	}
}
