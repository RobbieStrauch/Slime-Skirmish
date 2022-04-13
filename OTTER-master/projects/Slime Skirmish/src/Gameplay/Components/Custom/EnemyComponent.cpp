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

EnemyComponent::EnemyComponent() : IComponent()
{ }

EnemyComponent::~EnemyComponent() = default;

void EnemyComponent::Awake() { }

void EnemyComponent::RenderImGui() 
{ 
	LABEL_LEFT(ImGui::DragFloat, "Cooldown", &enemyAttackCooldown, 0);
}

nlohmann::json EnemyComponent::ToJson() const 
{
	return 
	{
		{ "cooldown", enemyAttackCooldown }
	};
}

EnemyComponent::Sptr EnemyComponent::FromJson(const nlohmann::json & blob) 
{
	EnemyComponent::Sptr result = std::make_shared<EnemyComponent>();
	result->enemyAttackCooldown = blob["cooldown"];
	return result;
}

void EnemyComponent::SetPlayer(Gameplay::GameObject::Sptr player)
{
	playerObject = player;
}

void EnemyComponent::Update(float deltaTime)
{
	const float safeDistance = 4.0f;
	glm::vec3 desiredDirection = playerObject->GetPosition() - GetGameObject()->GetPosition();

	// Enemy attack
	if (glfwGetTime() > enemyNextAttack)
	{
		if (GetGameObject()->Get<ArrivalBehaviour>()->GetType() == ArrivalType::Attack)
		{
			GetGameObject()->Get<MorphTarget>()->SetType(AnimationType::Attack);

			playerObject->Get<MovementComponent>()->isHit = true;
			playerObject->SetScale(playerObject->GetScale() - (GetGameObject()->GetSize() / 10.0f));
			playerObject->SetSize(playerObject->GetSize() - GetGameObject()->GetSize());
			enemyNextAttack = glfwGetTime() + enemyAttackCooldown;
		}
	}

	// Player eats
	if (GetGameObject()->GetSize() <= playerObject->GetSize())
	{
		if (GetGameObject()->Get<ArrivalBehaviour>()->GetType() == ArrivalType::Patrol && glm::length(desiredDirection) < safeDistance)
		{
			playerObject->Get<MovementComponent>()->isEat = true;
			playerObject->Score.x++;
			playerObject->SetScale(playerObject->GetScale() + (GetGameObject()->GetSize() / 10.0f));
			playerObject->SetSize(playerObject->GetSize() + GetGameObject()->GetSize());
			Application::Get().CurrentScene()->RemoveGameObject(GetGameObject()->SelfRef());
		}
	}
}
