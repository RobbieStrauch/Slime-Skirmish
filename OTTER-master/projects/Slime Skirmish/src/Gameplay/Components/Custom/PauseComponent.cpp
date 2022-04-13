#include "PauseComponent.h"
#include "Gameplay/Components/GUI/RectTransform.h"
#include "Gameplay/Components/GUI/GuiPanel.h"
#include "Gameplay/Components/GUI/GuiText.h"

#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "Application/Application.h"
#include "Application/Timing.h"

using namespace Gameplay;

PauseComponent::PauseComponent() : IComponent()
{}

PauseComponent::~PauseComponent() = default;

void PauseComponent::Awake()
{}

void PauseComponent::RenderImGui()
{
	LABEL_LEFT(ImGui::DragFloat, "Play Time", &playTime, 1.0f);
}

nlohmann::json PauseComponent::ToJson() const
{
	return
	{
		{ "play_time", playTime }
	};
}

PauseComponent::Sptr PauseComponent::FromJson(const nlohmann::json & blob)
{
	PauseComponent::Sptr result = std::make_shared<PauseComponent>();
	result->playTime = blob["play_time"];
	return result;
}

void PauseComponent::Update(float deltaTime)
{
	GameObject::Sptr titleText = GetGameObject()->GetChildren()[0];
	GameObject::Sptr resumeText = GetGameObject()->GetChildren()[1];
	GameObject::Sptr exitText = GetGameObject()->GetChildren()[2];
	GameObject::Sptr scoreText = GetGameObject()->GetChildren()[3];

	float middle = Application::Get().GetWindowSize().x / 2;
	float bottom = Application::Get().GetWindowSize().y;
	float middleY = Application::Get().GetWindowSize().y / 2 + 50;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

	if (glfwGetKey(Application::Get().GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		isPaused = !isPaused;
		pressed = true;
		glfwWaitEventsTimeout(2.0f);
	}
	playTime = isPaused ? 0.0f : 1.0f;
	if (pressed)
	{
		Timing::SetTimeScale(playTime);
	}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

	if (!isPaused)
	{
		GetGameObject()->Get<GuiPanel>()->SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	}
	else
	{
		GetGameObject()->Get<GuiPanel>()->SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.75f));
	}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

	if (pressed && isPaused)
	{
		float t = 0.1;

		if (t < 1.f)
		{
			t += deltaTime;

			titleText->Get<RectTransform>()->SetPosition(LERP(titleText->Get<RectTransform>()->GetPosition(), { middle, middleY - 200 }, t));
			resumeText->Get<RectTransform>()->SetPosition(LERP(resumeText->Get<RectTransform>()->GetPosition(), { middle, middleY }, t));
			exitText->Get<RectTransform>()->SetPosition(LERP(exitText->Get<RectTransform>()->GetPosition(), { middle, middleY + 100 }, t));

			scoreText->Get<RectTransform>()->SetPosition(LERP(scoreText->Get<RectTransform>()->GetPosition(), { middle, bottom - 150 }, t));
		}
		else pressed = false;
	}
	if (pressed && !isPaused)
	{
		float t = 0.1f;

		if (t < 1.f)
		{
			t += deltaTime;

			titleText->Get<RectTransform>()->SetPosition(LERP(titleText->Get<RectTransform>()->GetPosition(), { middle, -100 }, t));
			resumeText->Get<RectTransform>()->SetPosition(LERP(resumeText->Get<RectTransform>()->GetPosition(), { middle, -100 }, t));
			exitText->Get<RectTransform>()->SetPosition(LERP(exitText->Get<RectTransform>()->GetPosition(), { middle, -100 }, t));

			scoreText->Get<RectTransform>()->SetPosition(LERP(scoreText->Get<RectTransform>()->GetPosition(), { middle, bottom }, t));
		}
		else pressed = false;
	}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

	if (!isPaused && !pressed)
	{
		titleText->Get<RectTransform>()->SetPosition({ middle, -100 });
		resumeText->Get<RectTransform>()->SetPosition({ middle, -100 });
		exitText->Get<RectTransform>()->SetPosition({ middle, -100 });

		scoreText->Get<RectTransform>()->SetPosition({ middle, bottom });
	}
	if (isPaused && !pressed)
	{
		titleText->Get<RectTransform>()->SetPosition({ middle, middleY - 200 });
		resumeText->Get<RectTransform>()->SetPosition({ middle, middleY });
		exitText->Get<RectTransform>()->SetPosition({ middle, middleY + 100 });

		scoreText->Get<RectTransform>()->SetPosition({ middle, bottom - 150 });
	}
}


