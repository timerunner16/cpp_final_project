#include <optional>
#include <string>
#include <memory>
#include <vector>
#include "glcommon.hpp"
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
	float heightfloor=0;
	float heightceiling=0;
	std::vector<triangle> triangles;
};

class Map {
public:
	Map(Game* game, std::string mapname);
	~Map();

	std::vector<line> GetLines();
	std::vector<MapSegmentRenderData> GetMapSegments();
	std::optional<Sector> GetHighestFloorOverlapping(Box& box);
	std::optional<Sector> GetLowestCeilingOverlapping(Box& box);
private:
	std::vector<MapSegmentRenderData> m_map_segments;
	std::vector<line> m_lines;
	std::vector<Sector> m_sectors;

	Game* m_game;
};
