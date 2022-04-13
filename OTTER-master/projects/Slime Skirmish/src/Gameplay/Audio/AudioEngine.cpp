#include "AudioEngine.h"

AudioEngine* AudioEngine::Instance()
{
	// Singleton pointer
	static AudioEngine* p_Instance = nullptr;

	if (p_Instance == nullptr)
	{
		p_Instance = new AudioEngine();
	}

	return p_Instance;
}

int AudioEngine::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		//std::cerr << "Whoops, FMOD error: " << FMOD_ErrorString(result) << std::endl;
#ifdef DEBUG
		__debugbreak();
#endif
		return 1;
	}

	return 0;
}

void AudioEngine::Init()
{
	// Initilizes the audio systems in FMOD
	AudioEngine::ErrorCheck(FMOD::Studio::System::create(&m_StudioSystem));
	AudioEngine::ErrorCheck(m_StudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	// Save ref to core system
	AudioEngine::ErrorCheck(m_StudioSystem->getCoreSystem(&m_System));

	m_System->set3DNumListeners(1);

	// Forward and up vector can be created from your transformation matrices (world transform)
	/*
		X  Y  Z
		Rx Rx Rx Tx
		Ry Ry Ry Ty
		Rz Rz Rz Tz
		0  0  0  1
	
	*/
	FMOD_VECTOR up =		FMOD_VECTOR{ 0, 1, 0 };
	FMOD_VECTOR forward =	FMOD_VECTOR{ 0, 0, 1 };
	FMOD_VECTOR position =	FMOD_VECTOR{ 0, 0, 0 };
	FMOD_VECTOR velocity =	FMOD_VECTOR{ 0, 0, 0 };

	m_System->set3DListenerAttributes(0, &position, &velocity, &forward, &up);
}

void AudioEngine::Update()
{
	AudioEngine::ErrorCheck(m_StudioSystem->update());
}

void AudioEngine::Shutdown()
{
	ErrorCheck(m_System->close());
	ErrorCheck(m_System->release());
}

void AudioEngine::LoadSound(const std::string& soundName, const std::string& filename, bool b_3d, bool b_Looping, bool b_Stream)
{
	// Check if already loaded
	auto foundElement = m_SoundMap.find(soundName);
	if (foundElement != m_SoundMap.end())
	{
		// Has already been loaded
		return;
	}

	FMOD_MODE mode = FMOD_DEFAULT;
	mode |= (b_3d) ? FMOD_3D : FMOD_2D;
	mode |= (b_Looping) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	mode |= (b_Stream) ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* loadedSound;
	ErrorCheck(m_System->createSound(filename.c_str(), FMOD_3D, nullptr, &loadedSound));

	if (loadedSound != nullptr)
	{
		m_SoundMap[soundName] = loadedSound;
	}
}

void AudioEngine::UnloadSound(const std::string& soundName)
{
	// Check if already loaded
	auto foundElement = m_SoundMap.find(soundName);
	if (foundElement != m_SoundMap.end())
	{
		ErrorCheck(foundElement->second->release());
		m_SoundMap.erase(foundElement);
	}


}

void AudioEngine::PlaySoundByName(const std::string& soundName, FMOD::Channel** channelReturn)
{
	auto soundIt = m_SoundMap.find(soundName);
	if (soundIt == m_SoundMap.end())
	{
#ifdef DEBUG
		__debugbreak(); // Sound not found
		std::cerr << "Oh no, sound: " << soundName << " was not found!" << std::endl;
#endif
	}

	ErrorCheck(m_System->playSound((*soundIt).second, nullptr, false, channelReturn));
}

AudioEngine::AudioEngine()
{}