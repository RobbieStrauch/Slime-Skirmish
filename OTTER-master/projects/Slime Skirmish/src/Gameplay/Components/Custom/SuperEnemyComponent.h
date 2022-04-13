#pragma once
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/GameObject.h"

class SuperEnemyComponent : public Gameplay::IComponent {

public:
	typedef std::shared_ptr<SuperEnemyComponent> Sptr;

	SuperEnemyComponent();
	virtual ~SuperEnemyComponent();

	// Inherited from IComponent
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void RenderImGui() override;
	virtual nlohmann::json ToJson() const override;
	static SuperEnemyComponent::Sptr FromJson(const nlohmann::json& blob);
	MAKE_TYPENAME(SuperEnemyComponent);

protected:
	bool canSpawn = false;
	float spawnTimer = 0.0f;
	
};