#pragma once
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Audio/AudioSource.h"
#include "Gameplay/Audio/AudioEngine.h"
#include "Gameplay/Audio/AudioListener.h"

class FMODStudio : public Gameplay::IComponent {

public:
	typedef std::shared_ptr<FMODStudio> Sptr;

	FMODStudio();
	virtual ~FMODStudio();

	void SetSource(std::string soundName, bool loop, bool pause);
	AudioEngine* engine = AudioEngine::Instance();
	inline static AudioSource source;
	float volume = 0.0f;

	// Inherited from IComponent
	virtual void Awake() override;
	virtual void Update(float deltaTime) override;
	virtual void RenderImGui() override;
	virtual nlohmann::json ToJson() const override;
	static FMODStudio::Sptr FromJson(const nlohmann::json& blob);
	MAKE_TYPENAME(FMODStudio);

protected:
	
};