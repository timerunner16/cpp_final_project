#include "audio_segment.hpp"

AudioSegment::AudioSegment(uint8_t* data, uint32_t size) {
	SDL_RWops* rw = SDL_RWFromMem(data, size);
	SDL_LoadWAV_RW(rw, 0, &m_audio_spec, &m_audio_buf, &m_audio_len);
}

AudioSegment::~AudioSegment() {
	SDL_FreeWAV(m_audio_buf);
}

uint8_t* AudioSegment::GetAudioBuf() {return m_audio_buf;}
uint32_t AudioSegment::GetAudioLen() {return m_audio_len;}
SDL_AudioSpec AudioSegment::GetSpec() {return m_audio_spec;}
