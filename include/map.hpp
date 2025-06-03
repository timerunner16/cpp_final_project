#include <optional>
#include <string>
#include <memory>
#include <vector>
#include "GL/glew.h"
#include "GL/gl.h"
#include "physics.hpp"

#define SCALE (32.0f)
#define STEP_SIZE (12.0f/SCALE)

class Material;
class Game;

struct MapSegmentRenderData {
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	GLuint num_indices;
	std::shared_ptr<Material> material;
};

struct Sector {
	float heightfloor;
	float heightceiling;
	std::vector<triangle> triangles;
};

class Map {
public:
	Map(Game* game, std::string mapname);
	~Map();

	std::vector<line> GetLines();
	std::vector<MapSegmentRenderData> GetMapSegments();
	std::optional<Sector> GetHighestOverlappingSector(Box& box);
private:
	std::vector<MapSegmentRenderData> m_map_segments;
	std::vector<line> m_lines;
	std::vector<Sector> m_sectors;

	Game* m_game;
};
