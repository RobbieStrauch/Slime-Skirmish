#include "FMODStudio.h"

#include <GLFW/glfw3.h>
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Utils/ImGuiHelper.h"
#include "Application/Application.h"

FMODStudio::FMODStudio() : IComponent()
{ }

FMODStudio::~FMODStudio() = default;

void FMODStudio::Awake() { }

void FMODStudio::RenderImGui() { }

nlohmann::json FMODStudio::ToJson() const
{
	return
	{

	};
}

FMODStudio::Sptr FMODStudio::FromJson(const nlohmann::json & blob)
{
	FMODStudio::Sptr result = std::make_shared<FMODStudio>();
	return result;
}

void FMODStudio::SetSource(std::string soundName, bool loop, bool pause)
{
	source.SetSound(soundName);
	source.Play();
	source.SetLoop(loop);
	source.SetPause(pause);
	source.SetVolume(volume);
}

void FMODStudio::Update(float deltaTime)
{
	engine->Update();
}

