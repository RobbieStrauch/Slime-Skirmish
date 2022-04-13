#include "MovementComponent.h"
#include "AbilityComponent.h"
#include "FMODStudio.h"
#include "MorphTarget.h"

#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Gameplay/InputEngine.h"
#include "Utils/ImGuiHelper.h"
#include "Application/Application.h"

void MovementComponent::Awake()
{
	_body = GetComponent<Gameplay::Physics::RigidBody>();
	if (_body == nullptr)
	{
		IsEnabled = false;
	}
}

void MovementComponent::RenderImGui()
{
	LABEL_LEFT(ImGui::DragFloat, "Impulse", &_impulse, 1.0f);
}

nlohmann::json MovementComponent::ToJson() const
{
	return
	{
		{ "impulse", _impulse }
	};
}

MovementComponent::MovementComponent() : IComponent()
{

}

MovementComponent::~MovementComponent() = default;

MovementComponent::Sptr MovementComponent::FromJson(const nlohmann::json & blob)
{
	MovementComponent::Sptr result = std::make_shared<MovementComponent>();
	result->_impulse = blob["impulse"];
	return result;
}

void MovementComponent::Update(float deltaTime)
{
	if (InputEngine::IsKeyDown(GLFW_KEY_W))
	{
		isIdle = false;
		if (_isPressed == false)
		{
			_body->SetLinearVelocity(glm::vec3(0.0f, _impulse, 0.0f));
			rotate = glm::vec3(90.0f, 0.0f, -90.0f);
		}
		_isPressed = InputEngine::IsKeyDown(GLFW_KEY_W);
	}
	else
	{
		_isPressed = false;
	}

	if (InputEngine::IsKeyDown(GLFW_KEY_S))
	{
		isIdle = false;
		if (_isPressed == false)
		{
			_body->SetLinearVelocity(glm::vec3(0.0f, _impulse * -1.0f, 0.0f));
			rotate = glm::vec3(90.0f, 0.0f, 90.0f);
		}
		_isPressed = InputEngine::IsKeyDown(GLFW_KEY_S);
	}
	else
	{
		_isPressed = false;
	}

	if (InputEngine::IsKeyDown(GLFW_KEY_A))
	{
		isIdle = false;
		if (_isPressed == false)
		{
			_body->SetLinearVelocity(glm::vec3(_impulse * -1.0f, 0.0f, 0.0f));
			rotate = glm::vec3(90.0f, 0.0f, 0.0f);
		}
		_isPressed = InputEngine::IsKeyDown(GLFW_KEY_A);
	}
	else
	{
		_isPressed = false;
	}

	if (InputEngine::IsKeyDown(GLFW_KEY_D))
	{
		isIdle = false;
		if (_isPressed == false)
		{
			_body->SetLinearVelocity(glm::vec3(_impulse, 0.0f, 0.0f));
			rotate = glm::vec3(90.0f, 0.0f, 180.0f);
		}
		_isPressed = InputEngine::IsKeyDown(GLFW_KEY_D);
	}
	else
	{
		_isPressed = false;
	}

	if (InputEngine::IsKeyDown(GLFW_KEY_W) == false && InputEngine::IsKeyDown(GLFW_KEY_A) == false)
	{
		if (InputEngine::IsKeyDown(GLFW_KEY_S) == false && InputEngine::IsKeyDown(GLFW_KEY_D) == false)
		{
			isIdle = true;
			_body->SetLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}

	if (isEat)
	{
		GetGameObject()->Get<FMODStudio>()->SetSource("Eat", false, false);
		isEat = false;
	}
	if (isHit)
	{
		GetGameObject()->Get<FMODStudio>()->SetSource("Hit", false, false);
		isHit = false;
	}

	if ((InputEngine::GetKeyState(GLFW_KEY_W) == ButtonState::Pressed && !isIdle) ||
		(InputEngine::GetKeyState(GLFW_KEY_A) == ButtonState::Pressed && !isIdle) ||
		(InputEngine::GetKeyState(GLFW_KEY_S) == ButtonState::Pressed && !isIdle) ||
		(InputEngine::GetKeyState(GLFW_KEY_D) == ButtonState::Pressed && !isIdle))
	{
		GetGameObject()->Get<FMODStudio>()->SetSource("Move", false, false);
	}
	if (isIdle)
	{
		GetGameObject()->Get<FMODStudio>()->SetSource("Move", false, true);
	}

	GetGameObject()->SetRotation(rotate);
}

