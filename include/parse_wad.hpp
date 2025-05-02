#include <cstdint>
#include <string>

#define BAD_LUMP_DATA lumpdata{0,nullptr,false}

struct lumpinfo {
	uint32_t filepos;
	uint32_t size;
	std::string filename;
};

struct lumpdata {
	uint32_t size;
	uint8_t* data;
	bool successful;
};

std::string _8xU8tostr(uint8_t a, uint8_t b, uint8_t c, uint8_t d,
					   uint8_t e, uint8_t f, uint8_t g, uint8_t h);

uint32_t _4xU8to1xU32(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

lumpdata extract_lump_from_wad(std::string wad_path, std::string lumpname, int advance = 0);
