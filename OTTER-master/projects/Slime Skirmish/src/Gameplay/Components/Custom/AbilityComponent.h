#pragma once
#include <EnumToString.h>
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/Physics/RigidBody.h"

ENUM(AbilityType, int,
	None = 0,
	Idle = 1,
	Move = 2,
	Absorb = 3,
	Attack = 4,
	);

class AbilityComponent : public Gameplay::IComponent
{
public:
	typedef std::shared_ptr<AbilityComponent> Sptr;

	AbilityComponent(AbilityType type = AbilityType::None);

	void SetType(AbilityType newType);
	AbilityType GetType() const;

	virtual void RenderImGui() override;
	virtual nlohmann::json ToJson() const override;
	static AbilityComponent::Sptr FromJson(const nlohmann::json& blob);
	MAKE_TYPENAME(AbilityComponent);

private:
	AbilityType _type;


};

