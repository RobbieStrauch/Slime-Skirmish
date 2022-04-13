#pragma once
#include <EnumToString.h>
#include <unordered_map>
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/MeshResource.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Animations.h"

ENUM(AnimationType, int,
	None = 0,
	Idle = 1,
	Walk = 2,
	Run = 3,
	Attack = 4,
	Wind = 5,
);

class MorphTarget : public Gameplay::IComponent
{
public:
	typedef std::shared_ptr<MorphTarget> Sptr;

	MorphTarget();
	virtual ~MorphTarget();

	void SetType(AnimationType type);
	AnimationType GetType();

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void RenderImGui() override;
	virtual nlohmann::json ToJson() const override;
	static MorphTarget::Sptr FromJson(const nlohmann::json& blob);
	MAKE_TYPENAME(MorphTarget);

protected:
	AnimationType _type;
};

