#include <SDL2/SDL.h>
#include "game.hpp"

bool optionExists(char** begin, char** end, const std::string& option) {
	return std::find(begin, end, option) != end;
}

char* getOption(char** begin, char** end, const std::string& option) {
	char** iter = std::find(begin, end, option);
	if (iter != end && ++iter != end) return *iter;
	else return nullptr;
}

int main(int argc, char* argv[]) {
	std::string wad_path;
	int width = 320;
	int height = 240;
	int downscale = 1;

	char* width_c = getOption(argv, argv+argc, "-w");
	if (width_c != nullptr) width = atoi(width_c);

	char* height_c = getOption(argv, argv+argc, "-h");
	if (height_c != nullptr) height = atoi(height_c);

	char* downscale_c = getOption(argv, argv+argc, "-d");
	if (downscale_c != nullptr) downscale = atoi(downscale_c);

	bool resizable = false;
	if (optionExists(argv, argv+argc, "-r")) resizable = true;

	char* wad_path_c = getOption(argv, argv+argc, "-p");
	if (wad_path_c != nullptr) wad_path = std::string(wad_path_c);
	else wad_path = std::string("assets/GAME.wad");

	Game game = Game(wad_path, width, height, downscale, resizable);
	return 0;
}
