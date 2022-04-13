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
#include <fstream>

#include "Logging.h"
#include "imgui.h"

#include "EndSceneLayer.h"

using namespace Gameplay;
using namespace Gameplay::Physics;

EndSceneLayer::EndSceneLayer() : ApplicationLayer()
{
	Name = "End Scene";
	Overrides = AppLayerFunctions::OnAppLoad | AppLayerFunctions::OnUpdate;
}

EndSceneLayer::~EndSceneLayer() = default;

void EndSceneLayer::OnAppLoad(const nlohmann::json& config)
{
	_CreateScene();
}

void EndSceneLayer::OnUpdate()
{
	if (Application::Get().CurrentScene()->FindObjectByName("End Panel") != nullptr)
	{
		_MainLoop();
	}
}

void EndSceneLayer::DeclareButton(GameObject::Sptr textPanel, glm::vec2 mouse)
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

void EndSceneLayer::RestartButton(GameObject::Sptr restartButton, glm::vec2 mouse)
{
	DeclareButton(restartButton, mouse);
	double distance = glm::distance(mouse, restartButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		Application::Get().LoadScene("scenes/main-game.json");
		endReading = false;
	}
}

void EndSceneLayer::ExitButton(GameObject::Sptr exitButton, glm::vec2 mouse)
{
	DeclareButton(exitButton, mouse);
	double distance = glm::distance(mouse, exitButton->Get<RectTransform>()->GetPosition());

	if (distance < buttonDistance && InputEngine::GetMouseState(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::Pressed)
	{
		Application::Get().LoadScene("scenes/menu-screen.json");
		endReading = false;
	}
}

void EndSceneLayer::_CreateScene()
{
	Application& app = Application::Get();
	bool loadScene = false;

	if (loadScene && std::filesystem::exists("scene.json"))
	{
		app.LoadScene("scene.json");
	}
	else
	{
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

		// Create camera
		GameObject::Sptr camera = scene->MainCamera->GetGameObject()->SelfRef();
		{
			camera->SetPostion(glm::vec3(20.0f, 17.0f, 5.0f));
			camera->LookAt(glm::vec3(0.0f));
			camera->SetRotation(glm::vec3(-20.0f, 0.0f, 120.0f));
		}

		GameObject::Sptr endPanel = scene->CreateGameObject("End Panel");
		{
			RectTransform::Sptr transform = endPanel->Add<RectTransform>();
			transform->SetMin({ -12, -12 });
			transform->SetMax({ 2000, 2000 });

			GuiPanel::Sptr panel = endPanel->Add<GuiPanel>();
			panel->SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

			GameObject::Sptr scoreText = scene->CreateGameObject("Score Text");
			{
				RectTransform::Sptr transform = scoreText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x / 2, 450 });

				GuiPanel::Sptr panel = scoreText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = scoreText->Add<GuiText>();
				text->SetText("");
				text->SetColor(glm::vec4(1.0f));
				text->SetFont(font);
			}

			GameObject::Sptr restartText = scene->CreateGameObject("Restart Text");
			{
				RectTransform::Sptr transform = restartText->Add<RectTransform>();
				transform->SetMin({ 732, 443 });
				transform->SetMax({ 822, 457 });
				transform->SetPosition({ app.GetWindowSize().x / 2, 550 });

				GuiPanel::Sptr panel = restartText->Add<GuiPanel>();
				panel->SetColor(glm::vec4(0.0f));

				//panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 25.0f);
				font->Bake();

				GuiText::Sptr text = restartText->Add<GuiText>();
				text->SetText("Restart");
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

			endPanel->AddChild(scoreText);
			endPanel->AddChild(restartText);
			endPanel->AddChild(exitText);
		}

		GuiBatcher::SetDefaultTexture(ResourceManager::CreateAsset<Texture2D>("textures/ui-sprite.png"));
		GuiBatcher::SetDefaultBorderRadius(8);

		// Save the asset manifest for all the resources we just loaded
		ResourceManager::SaveManifest("end-screen-manifest.json");

		// Save the scene to a JSON file
		scene->Save("scenes/end-screen.json");

		// Send the scene to the application
		app.LoadScene(scene);
	}
}

void EndSceneLayer::_MainLoop()
{
	Application& app = Application::Get();
	Scene::Sptr scene = app.CurrentScene();
	Timing& time = Timing::Current();

	std::ifstream scoreFile;
	int reading;
	int scoreSize = 0;
	int scoreEaten = 0;
	int scoreTime = 0;
	int score[3];
	std::string scoreString;
	int count = 0;

	GameObject::Sptr scoreText = scene->FindObjectByName("Score Text");
	GameObject::Sptr restartText = scene->FindObjectByName("Restart Text");
	GameObject::Sptr exitText = scene->FindObjectByName("Exit Text");

	glm::vec2 mousePosition = glm::vec2(InputEngine::GetMousePos().x, InputEngine::GetMousePos().y);

	if (!endReading)
	{
		scoreFile.open("Score.txt");

		if (!scoreFile.is_open())
		{
			LOG_ERROR("File does not exist, please make sure the file exists or is in the right location");
		}

		while (!scoreFile.eof())
		{
			scoreFile >> reading;
			score[count] = reading;
			count++;
		}

		scoreEaten = score[0];
		scoreTime = score[1];
		scoreSize = score[2];

		scoreFile.close();

		scoreString = "Enemies Eaten: " + std::to_string(scoreEaten) + "\t\tTime Played: " + std::to_string(scoreTime) + "\t\tLargest Slime Size: " + std::to_string(scoreSize);
		scoreText->Get<GuiText>()->SetText(scoreString);

		endReading = true;
	}

	scoreText->Get<RectTransform>()->SetPosition({ app.GetWindowSize().x / 2, 450 });
	restartText->Get<RectTransform>()->SetPosition({ app.GetWindowSize().x / 2, 550 });
	exitText->Get<RectTransform>()->SetPosition({ app.GetWindowSize().x / 2, 650 });

	RestartButton(restartText, mousePosition);
	ExitButton(exitText, mousePosition);
}
