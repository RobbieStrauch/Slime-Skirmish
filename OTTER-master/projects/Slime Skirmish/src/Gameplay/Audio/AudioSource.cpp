#include "AudioSource.h"
#include "AudioEngine.h"

void AudioSource::Update()
{
	AudioEngine::ErrorCheck(p_Channel->set3DAttributes(&position, &velocity));
}

void AudioSource::Play()
{
	AudioEngine::Instance()->PlaySoundByName(name, &p_Channel);
	AudioEngine::ErrorCheck(p_Channel->setVolume(volume));
	AudioEngine::ErrorCheck(p_Channel->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
}

bool AudioSource::isPlaying()
{
	return false;
}

void AudioSource::SetSound(const std::string soundName)
{
	name = soundName;
}

void AudioSource::SetPosition(const FMOD_VECTOR pos)
{
	position = pos;
}

void AudioSource::SetVelocity(const FMOD_VECTOR vel)
{
	velocity = vel;
}

void AudioSource::SetPause(const bool isPaused)
{
	paused = isPaused;
	AudioEngine::ErrorCheck(p_Channel->setPaused(paused));
}

void AudioSource::SetVolume(const float value)
{
	volume = value;
	AudioEngine::ErrorCheck(p_Channel->setVolume(volume));
}

void AudioSource::SetLoop(const bool isLooping)
{
	loop = isLooping;
	AudioEngine::ErrorCheck(p_Channel->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));
}

std::string AudioSource::GetSound()
{
	return name;
}

FMOD_VECTOR AudioSource::GetPosition()
{
	return position;
}

FMOD_VECTOR AudioSource::GetVelocity()
{
	return velocity;
}

bool AudioSource::GetPause()
{
	return paused;
}

float AudioSource::GetVolume()
{
	return volume;
}

bool AudioSource::GetLoop()
{
	return loop;
}
