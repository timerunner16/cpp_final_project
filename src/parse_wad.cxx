#include <cstring>
#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>
#include "parse_wad.hpp"

std::string _8xU8tostr(uint8_t a, uint8_t b, uint8_t c, uint8_t d,
					   uint8_t e, uint8_t f, uint8_t g, uint8_t h) {
	std::string i("");
	i += std::string(1,(char)a);
	i += std::string(1,(char)b);
	i += std::string(1,(char)c);
	i += std::string(1,(char)d);
	i += std::string(1,(char)e);
	i += std::string(1,(char)f);
	i += std::string(1,(char)g);
	i += std::string(1,(char)h);
	i = i.substr(0, i.find_first_of((char)0x00));
	return i;
}

uint32_t _4xU8to1xU32(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
	return (((uint32_t)d << 25) |
			((uint32_t)c << 16) |
			((uint32_t)b << 8) |
			 (uint32_t)a);
}

lumpdata extract_lump_from_wad(std::string wad_path, std::string lumpname, std::string marker, bool ignore_not_found, int advance) {
	std::vector<uint8_t> binary(0);

	std::ifstream file(wad_path, std::ios::binary);
	if (!file.is_open()) {
		printf("Error: couldn't open wad file \"%s\"\n", wad_path.c_str());
		return BAD_LUMP_DATA;
	}
	if (lumpname.size() > 8) {
		printf("Error: invalid lumpname \"%s\", lumpname must be at most 8 characters\n", lumpname.c_str());
		return BAD_LUMP_DATA;
	}

	file.unsetf(std::ios::skipws);

	std::streampos file_size;
	file.seekg(0, std::ios::end);
	file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<uint8_t> buf(file_size);
	buf.insert(buf.begin(),
			   std::istream_iterator<uint8_t>(file),
			   std::istream_iterator<uint8_t>());

	uint32_t identifier = _4xU8to1xU32(buf[0x0], buf[0x1], buf[0x2], buf[0x3]);
	uint32_t lumpnumber = _4xU8to1xU32(buf[0x4], buf[0x5], buf[0x6], buf[0x7]);
	uint32_t dirpointer = _4xU8to1xU32(buf[0x8], buf[0x9], buf[0xA], buf[0xB]);
	if (identifier != _4xU8to1xU32('P','W','A','D')) {
		printf("Error: wad file \"%s\" is not a valid PWAD\n", wad_path.c_str());
		return BAD_LUMP_DATA;
	}
	if (lumpnumber == 0) {
		printf("Error: wad file \"%s\" doesn't have any lumps\n", wad_path.c_str());
		return BAD_LUMP_DATA;
	}
	
	std::vector<lumpinfo> lumps(0);
	size_t i = dirpointer;
	while (i < dirpointer + 16*lumpnumber) {
		lumps.push_back(lumpinfo{
			_4xU8to1xU32(buf[i+0x0], buf[i+0x1], buf[i+0x2], buf[i+0x3]),
			_4xU8to1xU32(buf[i+0x4], buf[i+0x5], buf[i+0x6], buf[i+0x7]),
			_8xU8tostr(buf[i+0x8], buf[i+0x9], buf[i+0xA], buf[i+0xB], buf[i+0xC], buf[i+0xD], buf[i+0xE], buf[i+0xF])
		});
		i+=16;
	}

	lumpinfo found{0,0,""};
	bool inrange = false;
	for (size_t i = 0; i < lumps.size(); i++) {
		if (!marker.empty()) {
			if (lumps[i].filename == marker + "_START") inrange = true;
			else if (lumps[i].filename == marker + "_END") inrange = false;
		}
		if (lumps[i].filename == lumpname && (inrange || marker.empty())) {
			found = lumps[i+advance];
			break;
		}
	}
	if (found.filepos == 0) {
		if (!ignore_not_found) printf("Error: couldn't find lump \"%s\" in wad file \"%s\"\n", lumpname.c_str(), wad_path.c_str());
		return BAD_LUMP_DATA;
	}

	uint8_t* lumpbuf = new uint8_t[found.size];
	memcpy(lumpbuf, buf.data()+found.filepos, found.size);
	return lumpdata{
		found.size,
		lumpbuf,
		true
	};
}
