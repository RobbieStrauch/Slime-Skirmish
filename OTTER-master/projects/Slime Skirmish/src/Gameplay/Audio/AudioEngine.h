#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_studio.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <math.h>

// This class implements the singleton design patter, which is a nice and convenient way to be lazy and used all the time in games.
// This design pattern allows global access to a single instance of a class, and guarantees there will only be one.
class AudioEngine
{
public:
	static AudioEngine* Instance();

	static int ErrorCheck(FMOD_RESULT result);

	void Init();
	void Update();
	void Shutdown();
	void LoadSound(const std::string& soundName, const std::string& filename, bool b_3d, bool b_Looping = false, bool b_Stream = false);
	void UnloadSound(const std::string& soundName);
	void PlaySoundByName(const std::string& soundName, FMOD::Channel** channelReturn = nullptr);

private:
	// Hide constructor
	AudioEngine();

	// FMOD Systems
	FMOD::Studio::System* m_StudioSystem = nullptr;
	FMOD::System* m_System = nullptr;

	// Sounds - map filename to sound object
	std::unordered_map<std::string, FMOD::Sound*> m_SoundMap;
};

