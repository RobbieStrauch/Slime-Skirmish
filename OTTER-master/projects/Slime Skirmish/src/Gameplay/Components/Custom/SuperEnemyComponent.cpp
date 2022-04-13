#include "SuperEnemyComponent.h"
#include "EnemyComponent.h"
#include "ArrivalBehaviour.h"
#include "AbilityComponent.h"
#include "MorphTarget.h"
#include "MovementComponent.h"

#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "Application/Application.h"

SuperEnemyComponent::SuperEnemyComponent() : IComponent()
{ }

SuperEnemyComponent::~SuperEnemyComponent() = default;

void SuperEnemyComponent::Awake() { }

void SuperEnemyComponent::RenderImGui()
{
	//LABEL_LEFT(ImGui::DragFloat, "Cooldown", &enemyAttackCooldown, 0);
}

nlohmann::json SuperEnemyComponent::ToJson() const
{
	return
	{
		//{ "cooldown", enemyAttackCooldown }
	};
}

SuperEnemyComponent::Sptr SuperEnemyComponent::FromJson(const nlohmann::json & blob)
{
	SuperEnemyComponent::Sptr result = std::make_shared<SuperEnemyComponent>();
	//result->enemyAttackCooldown = blob["cooldown"];
	return result;
}

void SuperEnemyComponent::Update(float deltaTime)
{
	if (canSpawn)
	{
		spawnTimer += deltaTime;
	}
}

