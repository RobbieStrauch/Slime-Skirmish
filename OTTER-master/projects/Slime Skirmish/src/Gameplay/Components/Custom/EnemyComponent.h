#pragma once
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/GameObject.h"

class EnemyComponent : public Gameplay::IComponent {

public:
	typedef std::shared_ptr<EnemyComponent> Sptr;

	EnemyComponent();
	virtual ~EnemyComponent();

	void SetPlayer(Gameplay::GameObject::Sptr player);

	// Inherited from IComponent
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void RenderImGui() override;
	virtual nlohmann::json ToJson() const override;
	static EnemyComponent::Sptr FromJson(const nlohmann::json& blob);
	MAKE_TYPENAME(EnemyComponent);

protected:
	Gameplay::GameObject::Sptr playerObject;
	float enemyNextAttack = 0.0f;
	float enemyAttackCooldown = 1.5f;
};