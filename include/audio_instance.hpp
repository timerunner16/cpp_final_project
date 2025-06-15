#pragma once
#include <memory>
#include <AL/al.h>

class AudioSegment;

class AudioInstance {
public:
	AudioInstance(std::shared_ptr<AudioSegment> audio_segment);
	~AudioInstance();

	void Play();
	void Pause();
	void SetVolume(float volume);
	void SetTrackPosition(float position);
	float GetPosition();
	bool IsFinished();
private:
	std::shared_ptr<AudioSegment> m_audio_segment;
	ALuint m_buffer;
	ALuint m_source;
};
