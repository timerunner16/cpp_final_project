#include "audio_instance.hpp"
#include "audio_segment.hpp"

AudioInstance::AudioInstance(std::shared_ptr<AudioSegment> audio_segment) {
	m_audio_segment = audio_segment;
	alDeleteBuffers(1, &m_buffer);
	alGenBuffers(1, &m_buffer);
	alBufferData(m_buffer, AL_FORMAT_STEREO16, m_audio_segment->GetAudioBuf(), m_audio_segment->GetAudioLen(), 44100);
	
	alDeleteSources(1, &m_source);
	alGenSources(1, &m_source);
	alSourcei(m_source, AL_BUFFER, m_buffer);
}

AudioInstance::~AudioInstance() {
	alDeleteSources(1, &m_source);
	alDeleteBuffers(1, &m_buffer);
}

void AudioInstance::Play() {
	alSourcePlay(m_source);
}

void AudioInstance::Pause() {
	alSourcePause(m_source);
}

void AudioInstance::SetVolume(float volume) {
	alSourcef(m_source, AL_GAIN, volume);
}

void AudioInstance::SetTrackPosition(float position) {
	alSourcei(m_source, AL_SAMPLE_OFFSET, (int)(position*44100));
}

float AudioInstance::GetPosition() {
	ALint samples;
	alGetSourcei(m_source, AL_SAMPLE_OFFSET, &samples);
	return samples/44100.0f;
}

bool AudioInstance::IsFinished() {
	ALenum finished;
	alGetSourcei(m_source, AL_SOURCE_STATE, &finished);
	return (finished == AL_STOPPED);
}
