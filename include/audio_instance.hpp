#pragma once
#include <memory>
#include <AL/al.h>

class AudioSegment;
class GameObject;
class Game;

class AudioInstance {
public:
	AudioInstance(std::shared_ptr<AudioSegment> audio_segment, GameObject* parent, Game* game);
	~AudioInstance();

	void Play();
	void Pause();
	float GetVolume();
	void SetVolume(float volume);
	float GetTrackPosition();
	void SetTrackPosition(float position);
	bool IsFinished();
	bool GetLooping();
	void SetLooping(bool looping);

	void Process();

	void SetParent(GameObject* parent);
private:
	std::shared_ptr<AudioSegment> m_audio_segment;
	ALuint m_buffer;
	ALuint m_source;
	GameObject* m_parent;
	Game* m_game;
};
