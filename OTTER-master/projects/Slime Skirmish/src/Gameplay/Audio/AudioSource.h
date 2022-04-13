#pragma once
#include "fmod.hpp"
#include "fmod_studio.hpp"

#include <iostream>
#include <string>

// Contains a sound and settings for playing that sound.
class AudioSource
{
public:
	void Update();
	void Play();
	bool isPlaying();
	void SetSound(const std::string soundName);
	void SetPosition(const FMOD_VECTOR pos);
	void SetVelocity(const FMOD_VECTOR vel);
	void SetPause(const bool isPaused);
	void SetVolume(const float value);
	void SetLoop(const bool isLooping);
	std::string GetSound();
	FMOD_VECTOR GetPosition();
	FMOD_VECTOR GetVelocity();
	bool GetPause();
	float GetVolume();
	bool GetLoop();

private:
	FMOD::Channel* p_Channel = nullptr;
	std::string name;
	FMOD_VECTOR position;
	FMOD_VECTOR velocity;
	float volume = 1.0f;
	bool loop = false;
	bool paused = true;
};
