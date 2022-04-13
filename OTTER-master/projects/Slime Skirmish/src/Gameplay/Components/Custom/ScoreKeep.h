#pragma once
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/GameObject.h"

class ScoreKeep : public Gameplay::IComponent
{
public:
	typedef std::shared_ptr<ScoreKeep> Sptr;

	ScoreKeep();
	virtual ~ScoreKeep();

	float nextSpawn = 0.0f;
	float spawnCooldown = 10.0f;
	double bestSize = 5;
	int enemyCounter = 0;
	int superEnemyCounter = 0;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void RenderImGui() override;
	virtual nlohmann::json ToJson() const override;
	static ScoreKeep::Sptr FromJson(const nlohmann::json& blob);
	MAKE_TYPENAME(ScoreKeep);

protected:

};

