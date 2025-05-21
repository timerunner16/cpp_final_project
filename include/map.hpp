#include <string>
#include <memory>
#include <vector>
#include "GL/glew.h"
#include "GL/gl.h"
#include "physics.hpp"

class Material;
class Game;

struct MapSegmentRenderData {
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	GLuint num_indices;
	std::shared_ptr<Material> material;
};

class Map {
public:
	Map(Game* game, std::string mapname);
	~Map();

	std::vector<line> GetLines();
	std::vector<MapSegmentRenderData> GetMapSegments();
private:
	std::vector<MapSegmentRenderData> m_map_segments;
	std::vector<line> m_lines;

	Game* m_game;
};
