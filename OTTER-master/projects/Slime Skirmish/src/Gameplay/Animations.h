#pragma once

#include "GameObject.h"
#include "MeshResource.h"
#include "Components/RenderComponent.h"

using namespace Gameplay;

class Animations
{
public:
	Animations();
	~Animations();

	void SetTime(float value);
	float GetTime()
	{
		return frameTime;
	}
	float GetLerpTime()
	{
		return timer;
	}
	void SetAnimation(std::string file, int size);
	void UpdateAnimation(GameObject::Sptr object, double deltaTime);

protected:
	std::vector<MeshResource::Sptr> frames;
	int frameSize = 0;
	float timer = 0.0f;
	float frameTime = 0.0f;
	int frameIndex = 0;

};

