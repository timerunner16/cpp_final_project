#pragma once
#include <SDL2/SDL.h>
#include "resource_manager.hpp"

class AudioSegment : public GenericResource {
public:
	AudioSegment(uint8_t* data, uint32_t size);
	~AudioSegment();

	SDL_AudioSpec GetSpec();
	uint8_t* GetAudioBuf();
	uint32_t GetAudioLen();
private:
	SDL_AudioSpec m_audio_spec;
	uint8_t* m_audio_buf;
	uint32_t m_audio_len;
};
