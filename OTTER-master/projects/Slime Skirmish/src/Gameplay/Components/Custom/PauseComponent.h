#pragma once
#include "Gameplay/Components/IComponent.h"

class PauseComponent : public Gameplay::IComponent {
public:
	typedef std::shared_ptr<PauseComponent> Sptr;

	PauseComponent();
	virtual ~PauseComponent();

	template<typename T>
	T LERP(const T& p0, const T& p1, float t)
	{
		return (1.0f - t) * p0 + t * p1;
	}
	bool isPaused = false;
	bool pressed = false;

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void RenderImGui() override;
	MAKE_TYPENAME(PauseComponent);
	virtual nlohmann::json ToJson() const override;
	static PauseComponent::Sptr FromJson(const nlohmann::json& blob);

protected:
	float playTime;
};