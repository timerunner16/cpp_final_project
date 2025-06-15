#include "audio_instance.hpp"
#include "audio_segment.hpp"

//#define MACROPRINT
#define VERBOSE_DBPRINTF
#include "macroprint.h"

void displayALError(ALenum error) {
	switch (error) {
		case AL_NO_ERROR:
			break;
		case AL_INVALID_NAME:
			DBPRINTF("AL: Invalid name\n");
			break;
		case AL_INVALID_ENUM:
			DBPRINTF("AL: Invalid enum\n");
			break;
		case AL_INVALID_VALUE:
			DBPRINTF("AL: Invalid value\n");
			break;
		case AL_INVALID_OPERATION:
			DBPRINTF("AL: Invalid operation\n");
			break;
		case AL_OUT_OF_MEMORY:
			DBPRINTF("AL: Out of memory\n");
			break;
	};
}

AudioInstance::AudioInstance(std::shared_ptr<AudioSegment> audio_segment) {
	m_audio_segment = audio_segment;
	alGenBuffers(1, &m_buffer);
	displayALError(alGetError());
	alBufferData(m_buffer, AL_FORMAT_STEREO16, m_audio_segment->GetAudioBuf(), m_audio_segment->GetAudioLen(), 44100);
	displayALError(alGetError());
	
	alGenSources(1, &m_source);
	displayALError(alGetError());
	alSourcei(m_source, AL_BUFFER, m_buffer);
	displayALError(alGetError());
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

float AudioInstance::GetVolume() {
	float volume;
	alGetSourcef(m_source, AL_GAIN, &volume);
	return volume;
}

void AudioInstance::SetVolume(float volume) {
	alSourcef(m_source, AL_GAIN, volume);
}

float AudioInstance::GetTrackPosition() {
	ALint samples;
	alGetSourcei(m_source, AL_SAMPLE_OFFSET, &samples);
	return samples/44100.0f;
}

void AudioInstance::SetTrackPosition(float position) {
	alSourcei(m_source, AL_SAMPLE_OFFSET, (int)(position*44100));
}

bool AudioInstance::IsFinished() {
	ALenum finished;
	alGetSourcei(m_source, AL_SOURCE_STATE, &finished);
	return (finished == AL_STOPPED);
}
