#include "TriggerVolumeEnterBehaviour.h"

#include "Gameplay/Components/ComponentManager.h"
#include "Gameplay/GameObject.h"

TriggerVolumeEnterBehaviour::TriggerVolumeEnterBehaviour() : IComponent(), _playerInTrigger(false)
{ }

TriggerVolumeEnterBehaviour::~TriggerVolumeEnterBehaviour() = default;

void TriggerVolumeEnterBehaviour::SetPlayerTrigger(bool change) 
{
	_playerInTrigger = change;
}

bool TriggerVolumeEnterBehaviour::GetPlayerTrigger() 
{
	return _playerInTrigger;
}

void TriggerVolumeEnterBehaviour::SetSpawnTrigger(bool change)
{
	_spawnInTrigger = change;
}

bool TriggerVolumeEnterBehaviour::GetSpawnTrigger()
{
	return _spawnInTrigger;
}

void TriggerVolumeEnterBehaviour::OnTriggerVolumeEntered(const std::shared_ptr<Gameplay::Physics::RigidBody>&body) 
{
	if (body->GetGameObject()->Name == "Player")
	{
		_playerInTrigger = true;
	}
	if ((GetGameObject()->Name == "Spawn0" || GetGameObject()->Name == "Spawn1" || GetGameObject()->Name == "Spawn2" || GetGameObject()->Name == "Spawn3") && body->GetGameObject()->Name == "Player")
	{
		_spawnInTrigger = false;
	}
}

void TriggerVolumeEnterBehaviour::OnTriggerVolumeLeaving(const std::shared_ptr<Gameplay::Physics::RigidBody>&body) 
{
	if (body->GetGameObject()->Name == "Player")
	{
		_playerInTrigger = false;
	}
	if ((GetGameObject()->Name == "Spawn0" || GetGameObject()->Name == "Spawn1" || GetGameObject()->Name == "Spawn2" || GetGameObject()->Name == "Spawn3") && body->GetGameObject()->Name == "Player")
	{
		_spawnInTrigger = true;
	}
}

void TriggerVolumeEnterBehaviour::RenderImGui() { }

nlohmann::json TriggerVolumeEnterBehaviour::ToJson() const 
{
	return { };
}

TriggerVolumeEnterBehaviour::Sptr TriggerVolumeEnterBehaviour::FromJson(const nlohmann::json & blob) 
{
	TriggerVolumeEnterBehaviour::Sptr result = std::make_shared<TriggerVolumeEnterBehaviour>();
	return result;
}
