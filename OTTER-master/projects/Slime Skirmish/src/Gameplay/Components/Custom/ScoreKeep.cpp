#include "ScoreKeep.h"
#include "Gameplay/Components/GUI/GuiText.h"
#include "Gameplay/Components/GUI/GuiPanel.h"
#include "Gameplay/Components/GUI/RectTransform.h"

#include <fstream>
#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "Application/Application.h"

using namespace Gameplay;

ScoreKeep::ScoreKeep() : IComponent()
{}

ScoreKeep::~ScoreKeep() = default;

void ScoreKeep::Awake()
{}

void ScoreKeep::RenderImGui()
{
	LABEL_LEFT(ImGui::DragFloat3, "Cooldown", &spawnCooldown);
}

nlohmann::json ScoreKeep::ToJson() const
{
	return
	{
		{ "cooldown", spawnCooldown }
	};
}

ScoreKeep::Sptr ScoreKeep::FromJson(const nlohmann::json & blob)
{
	ScoreKeep::Sptr result = std::make_shared<ScoreKeep>();
	result->spawnCooldown = blob["cooldown"];
	return result;
}

void ScoreKeep::Update(float deltaTime)
{
	GetGameObject()->Score.z = GetGameObject()->GetSize();

	if (GetGameObject()->Score.z > bestSize) bestSize = GetGameObject()->Score.z;
	GetGameObject()->Score.y += deltaTime;

	if (GetGameObject()->Score.z <= 0)
	{
		std::ofstream scoreFile;
		scoreFile.open("Score.txt");
		if (!scoreFile.is_open())
		{
			LOG_ERROR("File does not exist, please make sure the file exists or is in the right location");
		}
		scoreFile << int(GetGameObject()->Score.x) << "\n";
		scoreFile << int(GetGameObject()->Score.y) << "\n";
		scoreFile << int(bestSize);
		scoreFile.close();

		GetGameObject()->Score.y = 0;
		GetGameObject()->Score.x = 0;
		GetGameObject()->Score.z = 0;
		bestSize = 5;
		nextSpawn = 0.0f;
		enemyCounter = 0;

		Application::Get().LoadScene("scenes/end-screen.json");
	}
}

