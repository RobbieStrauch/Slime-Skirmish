#pragma once
#include <EnumToString.h>
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/GameObject.h"

ENUM(ArrivalType, int,
	Patrol = 0,
	Attack = 1,
	);

class ArrivalBehaviour : public Gameplay::IComponent
{
public:
	typedef std::shared_ptr<ArrivalBehaviour> Sptr;

	ArrivalBehaviour();
	virtual ~ArrivalBehaviour();

	void SetPlayer(Gameplay::GameObject::Sptr player);
	void SetWalls(std::vector<Gameplay::GameObject::Sptr> walls);
	void SetType(ArrivalType newType);
	ArrivalType GetType();

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void RenderImGui() override;
	virtual nlohmann::json ToJson() const override;
	static ArrivalBehaviour::Sptr FromJson(const nlohmann::json& blob);
	MAKE_TYPENAME(ArrivalBehaviour);

protected:
	Gameplay::GameObject::Sptr playerObject;
	std::vector<Gameplay::GameObject::Sptr> wallObjects; 
	int w_i;
	float enemyVelocity;
	glm::vec3 enemyMovement;
	ArrivalType type;
};

