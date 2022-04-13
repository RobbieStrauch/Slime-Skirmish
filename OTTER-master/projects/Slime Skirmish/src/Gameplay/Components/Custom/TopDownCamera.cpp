#include "TopDownCamera.h"

#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "Application/Application.h"

using namespace Gameplay;

TopDownCamera::TopDownCamera() : IComponent()
{}

TopDownCamera::~TopDownCamera() = default;

void TopDownCamera::SetCamera(GameObject::Sptr cam)
{
	camera = cam;
}

void TopDownCamera::SetHeight(float value)
{
	_height = value;
}

float TopDownCamera::GetHeight()
{
	return _height;
}

void TopDownCamera::SetDistance(float value)
{
	_distance = value;
}

float TopDownCamera::GetDistance()
{
	return _distance;
}

void TopDownCamera::Awake()
{
}

void TopDownCamera::RenderImGui()
{
	LABEL_LEFT(ImGui::DragFloat, "Height", &_height, 1.0f);
	LABEL_LEFT(ImGui::DragFloat, "Distance", &_distance, 1.0f);
}

nlohmann::json TopDownCamera::ToJson() const
{
	nlohmann::json result =
	{
		{ "height", _height },
		{ "distance", _distance }
	};
	return result;
}

TopDownCamera::Sptr TopDownCamera::FromJson(const nlohmann::json & blob)
{
	TopDownCamera::Sptr result = std::make_shared<TopDownCamera>();
	result->_height = blob["height"];
	result->_distance = blob["distance"];
	return result;
}

void TopDownCamera::Update(float deltaTime)
{
	_position = glm::vec3(0.0f, -_distance, _height);
	//std::cout << GetGameObject()->GetPosition().x << ", " << GetGameObject()->GetPosition().y << ", " << GetGameObject()->GetPosition().z << std::endl;
	//camera->SetPostion(/*GetGameObject()->GetPosition() + */_position);
	//camera->LookAt(GetGameObject()->GetPosition());
}
