#include "Animations.h"

Animations::Animations()
{}

Animations::~Animations() = default;

void Animations::SetTime(float value)
{
	frameTime = value;
}

void Animations::SetAnimation(std::string file, int size)
{
	std::string prefix = "models/animations/" + file;
	std::string filename;
	frameSize = size;

	for (int i = 1; i <= frameSize; ++i)
	{
		filename = prefix + std::to_string(i) + ".obj";
		MeshResource::Sptr animationMesh = ResourceManager::CreateAsset<MeshResource>(filename);
		frames.push_back(std::move(animationMesh));
	}
}

void Animations::UpdateAnimation(GameObject::Sptr object, double deltaTime)
{
	timer += deltaTime;

	while (timer > frameTime)
	{
		timer -= frameTime;
		frameIndex += 1;
	}
	if (frameIndex >= frames.size())
	{
		frameIndex = 0;
	}

	object->Get<RenderComponent>()->SetMesh(frames[frameIndex]);
}
