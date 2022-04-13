#include "ArrivalBehaviour.h"
#include "AbilityComponent.h"
#include "TriggerVolumeEnterBehaviour.h"
#include "EnemyComponent.h"
#include "MorphTarget.h"

#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "Application/Application.h"

using namespace Gameplay;

ArrivalBehaviour::ArrivalBehaviour() : IComponent(), enemyVelocity(2.0f)
{}

ArrivalBehaviour::~ArrivalBehaviour() = default;

void ArrivalBehaviour::Awake()
{

}

void ArrivalBehaviour::RenderImGui()
{
	LABEL_LEFT(ImGui::DragFloat, "Velocity", &enemyVelocity, 1.0f);
}

nlohmann::json ArrivalBehaviour::ToJson() const
{
	return
	{
		{ "velocity", enemyVelocity }
	};
}

ArrivalBehaviour::Sptr ArrivalBehaviour::FromJson(const nlohmann::json & blob)
{
	ArrivalBehaviour::Sptr result = std::make_shared<ArrivalBehaviour>();
	result->enemyVelocity = blob["velocity"];
	return result;
}

void ArrivalBehaviour::SetPlayer(Gameplay::GameObject::Sptr player)
{
	playerObject = player;
}

void ArrivalBehaviour::SetWalls(std::vector<Gameplay::GameObject::Sptr> walls)
{
	wallObjects.resize(walls.size());

	for (size_t i = 0; i < walls.size(); i++)
	{
		wallObjects[i] = walls[i];
	}

	w_i = rand() % wallObjects.size();
}

void ArrivalBehaviour::SetType(ArrivalType newType)
{
	type = newType;
}

ArrivalType ArrivalBehaviour::GetType()
{
	return type;
}

void ArrivalBehaviour::Update(float deltaTime)
{
	const float safeDistance = 8.0f;
	glm::vec3 desiredDirection = playerObject->GetPosition() - GetGameObject()->GetPosition();

	if (GetGameObject()->Get<TriggerVolumeEnterBehaviour>()->GetPlayerTrigger())
	{
		if (glm::length(desiredDirection) < 1)
		{
			enemyVelocity = 0.0f;
		}
		else
		{
			enemyVelocity = 1.5f;
		}

		type = ArrivalType::Attack;

		if (wallObjects[0]->Name == "Wall0")
		{
			GetGameObject()->LookAt(2.0f * GetGameObject()->GetPosition() - playerObject->GetPosition());
		}
		if (wallObjects[0]->Name == "Super Wall0")
		{
			GetGameObject()->LookAt(playerObject->GetPosition());
		}

		enemyMovement = glm::normalize(desiredDirection);
		enemyMovement *= enemyVelocity * deltaTime;
		GetGameObject()->SetPostion(GetGameObject()->GetPosition() + enemyMovement);
	}
	if (!GetGameObject()->Get<TriggerVolumeEnterBehaviour>()->GetPlayerTrigger())
	{
		enemyVelocity = 1.5f;

		glm::vec3 randomDirection = wallObjects[w_i]->GetPosition() - GetGameObject()->GetPosition();
		const float randomDistance = 11.5f;

		if (glm::length(randomDirection) < randomDistance)
		{
			w_i = rand() % wallObjects.size();
		}

		glm::vec3 test1 = GetGameObject()->GetPosition(); test1.z = 0.0f;
		glm::vec3 test2 = randomDirection + GetGameObject()->GetPosition(); test2.z = 0.0f;

		type = ArrivalType::Patrol;
		GetGameObject()->Get<MorphTarget>()->SetType(AnimationType::Walk);

		if (wallObjects[0]->Name == "Wall0")
		{
			GetGameObject()->LookAt(2.0f * test1 - test2);
		}
		if (wallObjects[0]->Name == "Super Wall0")
		{
			GetGameObject()->LookAt(test2);
		}

		enemyMovement = glm::normalize(randomDirection);
		enemyMovement *= enemyVelocity * deltaTime; enemyMovement.z = 0.0f;
		GetGameObject()->SetPostion(GetGameObject()->GetPosition() + enemyMovement);
	}
}
