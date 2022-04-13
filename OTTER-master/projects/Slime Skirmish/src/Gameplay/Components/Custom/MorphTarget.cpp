#include "MorphTarget.h"
#include "AbilityComponent.h"
#include "TriggerVolumeEnterBehaviour.h"
#include "EnemyComponent.h"

#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "Application/Application.h"

using namespace Gameplay;

MorphTarget::MorphTarget() : IComponent(), _type(AnimationType::None)
{}

MorphTarget::~MorphTarget() = default;

void MorphTarget::Awake()
{
}

void MorphTarget::RenderImGui()
{
	//LABEL_LEFT(ImGui::DragFloat, "Frame Time", &frameTime, 1.0f);
}

nlohmann::json MorphTarget::ToJson() const
{
	nlohmann::json result = 
	{
		//{ "frame_time", frameTime }
	};
	return result;
}

MorphTarget::Sptr MorphTarget::FromJson(const nlohmann::json & blob)
{
	MorphTarget::Sptr result = std::make_shared<MorphTarget>();
	//result->frameTime = blob["frame_time"];
	return result;
}

void MorphTarget::SetType(AnimationType type)
{
	_type = type;
}

AnimationType MorphTarget::GetType()
{
	return _type;
}

void MorphTarget::Update(float deltaTime)
{
	//timer += deltaTime;

	//while (timer > frameTime)
	//{
	//	timer -= frameTime;
	//	frameIndex += 1;

	//	if (frameIndex >= frames.size())
	//	{
	//		frameIndex = 0;
	//	}
	//}

	//GetGameObject()->Get<RenderComponent>()->SetMesh(frames[frameIndex]);
}