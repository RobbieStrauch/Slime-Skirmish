#pragma once
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"
#include "Application/ApplicationLayer.h"

class TopDownCamera : public Gameplay::IComponent
{
public:
	typedef std::shared_ptr<TopDownCamera> Sptr;

	TopDownCamera();
	virtual ~TopDownCamera();

	void SetCamera(Gameplay::GameObject::Sptr cam);
	void SetHeight(float value);
	float GetHeight();
	void SetDistance(float value);
	float GetDistance();

	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void RenderImGui() override;
	virtual nlohmann::json ToJson() const override;
	static TopDownCamera::Sptr FromJson(const nlohmann::json& blob);
	MAKE_TYPENAME(TopDownCamera);

protected:
	Gameplay::GameObject::Sptr camera;
	float _height = 0.0f;
	float _distance = 0.0f;
	glm::vec3 _position;
};

