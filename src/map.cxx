#include <algorithm>
#include <cmath>
#include <tpp_interface.hpp>
#include "map.hpp"
#include "game.hpp"
#include "material.hpp"
#include "physics.hpp"
#include "resource_manager.hpp"
#include "parse_wad.hpp"
#include "tpp_iterators.hpp"
#include "transform.hpp"
#include "util.hpp"
#include "mesh.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "workspace.hpp"
using namespace tpp;

#define SCALE (32.0f)

struct udmf_vertex {
	float x;
	float y;

	udmf_vertex() {
		x = 0;
		y = 0;
	}
	bool operator==(udmf_vertex obj) const {
		return obj.x == x && obj.y == y;
	}
};

struct udmf_linedef {
	int v1;
	int v2;
	int sidefront;
	int sideback;
	bool twosided;

	udmf_linedef() {
		v1 = 0;
		v2 = 0;
		sidefront = -1;
		sideback = -1;
		twosided = false;
	}
};

struct udmf_sidedef {
	int offsetx_top;
	int offsety_top;
	int offsetx_bottom;
	int offsety_bottom;
	int offsetx_mid;
	int offsety_mid;
	std::string texturetop;
	std::string texturebottom;
	std::string texturemiddle;
	int sector;

	udmf_sidedef() {
		offsetx_top = 0;
		offsety_top = 0;
		offsetx_bottom = 0;
		offsety_bottom = 0;
		offsetx_mid = 0;
		offsety_mid = 0;
		texturetop = "";
		texturebottom = "";
		texturemiddle = "";
		sector = 0;
	}
};

struct udmf_sector {
	int heightfloor;
	int heightceiling;
	std::string texturefloor;
	std::string textureceiling;
	int lightlevel;

	udmf_sector() {
		heightfloor = 0;
		heightceiling = 0;
		texturefloor = "";
		textureceiling = "";
		lightlevel = 160;
	}
};

struct udmf_thing {
	float x;
	float y;
	float height;
	int angle;
	std::string data;

	udmf_thing() {
		x = 0;
		y = 0;
		height = 0;
		angle = 0;
		data = "";
	}
};

struct tri_vertex {
	float x, y;
	tri_vertex() {
		x = y = 0;
	}
	tri_vertex(float x, float y) {
		this->x = x;
		this->y = y;
	}
	bool operator==(tri_vertex obj) const {
		return obj.x == x && obj.y == y;
	}
};

struct tri_edge {
	tri_vertex v0, v1;
	tri_edge() {
		v0 = v1 = tri_vertex();
	}
	tri_edge(tri_vertex v0, tri_vertex v1) {
		this->v0 = v0;
		this->v1 = v1;
	}
	bool operator==(tri_edge obj) const {
		return obj.v0 == v0 && obj.v1 == v1;
	}
};

struct tri_triangle {
	tri_vertex v0, v1, v2;
	tri_triangle() {
		v0 = v1 = v2 = tri_vertex();
	}
	tri_triangle(tri_vertex v0, tri_vertex v1, tri_vertex v2) {
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
	}
	bool operator==(tri_triangle obj) const {
		return obj.v0 == v0 && obj.v1 == v1 && obj.v2 == v2;
	}
};

void addmapsegment(std::vector<MapSegmentRenderData>& map_segments, mesh_vertex* vertex_data, GLuint* index_data, const GLuint& num_indices, std::shared_ptr<Material> material) {
	GLuint vertex_array_object, vertex_buffer_object, index_buffer_object;
	
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh_vertex)*num_indices, vertex_data, GL_STATIC_DRAW);

	glGenBuffers(1, &index_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*num_indices, index_data, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), (void*)(sizeof(glm::vec3)+sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	map_segments.push_back(MapSegmentRenderData{
		.vao=vertex_array_object,
		.vbo=vertex_buffer_object,
		.ibo=index_buffer_object,
		.num_indices=num_indices,
		.material=material
	});
}

struct vertexloop_t {
	std::vector<tri_vertex> vertices;
	float area;

	bool operator<(const vertexloop_t& obj) const {return area < obj.area;}
};

std::vector<tri_triangle> edges_to_faces(std::vector<tri_edge> edges) {
	if (edges.size() <= 2) return std::vector<tri_triangle>();
	
	std::vector<vertexloop_t> vertexloops(1);
	bool clear = false;
	while (!clear) {
		clear = true;
		std::vector<tri_edge> to_delete(0);
		for (size_t i = 0; i < edges.size(); i++) {
			tri_vertex v0 = edges[i].v0;
			tri_vertex v1 = edges[i].v1;
			bool v0matched = false;
			bool v1matched = false;
			for (size_t j = 0; j < edges.size(); j++) {
				if (i == j) continue;

				if (edges[j].v0 == v0 || edges[j].v1 == v0) v0matched = true;
				if (edges[j].v0 == v1 || edges[j].v1 == v1) v1matched = true;
			}
			if (!v0matched || !v1matched) {
				clear = false;
				to_delete.push_back(edges[i]);
			}
		}
		std::erase_if(edges, [&to_delete](const tri_edge& edge) -> bool {
			for (auto i : to_delete) {
				if (i == edge) return true;
			}
			return false;
		});
	}

	while (edges.size() > 0) {
		tri_vertex start = edges[0].v0;
		tri_vertex next = edges[0].v1;
		vertexloops[vertexloops.size()-1].vertices.push_back(start);
		vertexloops[vertexloops.size()-1].vertices.push_back(next);
		edges.erase(edges.begin());
		while (next != start && edges.size() > 0) {
			for (size_t i = 0; i < edges.size(); i++) {
				bool found = false;
				if (edges[i].v0 == next) {
					next = edges[i].v1;
					found = true;
				} else if (edges[i].v1 == next) {
					next = edges[i].v0;
					found = true;
				}
				if (found) {
					edges.erase(std::next(edges.begin(), i));
					break;
				}
			}
			if (next != start) vertexloops[vertexloops.size()-1].vertices.push_back(next);
		}
		if (edges.size() > 0) vertexloops.push_back(vertexloop_t());
	}

	std::erase_if(vertexloops, [](const vertexloop_t& vertexloop) -> bool {
		return vertexloop.vertices.size() < 3;
	});
	
	for (size_t i = 0; i < vertexloops.size(); i++) {
		vertexloop_t vertexloop = vertexloops[i];
		std::vector<Delaunay::Point> points;
		for (auto j : vertexloop.vertices) points.push_back(Delaunay::Point{j.x, j.y});
		Delaunay generator(points);
		generator.Triangulate();

		float area = 0.0f;
		for (FaceIterator fit = generator.fbegin(); fit != generator.fend(); ++fit) {
			Delaunay::Point p1, p2, p3;
			int v1 = fit.Org();
			int v2 = fit.Dest();
			int v3 = fit.Apex();
			if (v1 < 0) fit.Org(&p1);
			else p1 = points[v1];
			if (v2 < 0) fit.Dest(&p2);
			else p2 = points[v2];
			if (v3 < 0) fit.Apex(&p3);
			else p3 = points[v3];

			tri_triangle triangle(tri_vertex(p1[0],p1[1]), tri_vertex(p2[0],p2[1]), tri_vertex(p3[0],p3[1]));
			float p01 = sqrt(pow(triangle.v1.x-triangle.v0.x, 2.0) + pow(triangle.v1.y-triangle.v0.y, 2.0));
			float p12 = sqrt(pow(triangle.v2.x-triangle.v1.x, 2.0) + pow(triangle.v2.y-triangle.v1.y, 2.0));
			float p02 = sqrt(pow(triangle.v2.x-triangle.v0.x, 2.0) + pow(triangle.v2.y-triangle.v0.y, 2.0));
			float semiperimeter = (p01+p12+p02)/2;
			area += sqrt(semiperimeter * (semiperimeter - p01) * (semiperimeter - p12) * (semiperimeter - p02));
		}
		vertexloops[i].area = area;
	}

	std::sort(vertexloops.begin(), vertexloops.end());

	std::vector<Delaunay::Point> points;
	std::vector<Delaunay::Point> holes;
	std::vector<int> segments;
	size_t offset = 0;
	for (size_t i = 0; i < vertexloops.size(); i++) {
		vertexloop_t vertexloop = vertexloops[i];
		if (i < vertexloops.size()-1) {
			vec2 holecenter = vec2();
			for (tri_vertex j : vertexloop.vertices) {
				vec2 point(j.x, j.y);
				holecenter = holecenter + point/float(vertexloop.vertices.size());
		}
			holes.push_back(Delaunay::Point{holecenter.x, holecenter.y});
		}
		
		for (size_t j = 0; j < vertexloop.vertices.size(); j++) {
			points.push_back(Delaunay::Point{vertexloop.vertices[j].x, vertexloop.vertices[j].y});
			segments.push_back(offset+j);
			if (j == vertexloop.vertices.size()-1) segments.push_back(offset);
			else segments.push_back(offset+j+1);
		}
		offset += vertexloop.vertices.size();
	}
	
	Delaunay generator(points);
	generator.setHolesConstraint(holes);
	generator.setSegmentConstraint(segments);
	generator.Triangulate();
	
	std::vector<tri_triangle> output;
	
	for (FaceIterator fit = generator.fbegin(); fit != generator.fend(); ++fit) {
		Delaunay::Point p1, p2, p3;
		int v1 = fit.Org();
		int v2 = fit.Dest();
		int v3 = fit.Apex();
		if (v1 < 0) fit.Org(&p1);
		else p1 = points[v1];
		if (v2 < 0) fit.Dest(&p2);
		else p2 = points[v2];
		if (v3 < 0) fit.Apex(&p3);
		else p3 = points[v3];

		if (vec3(0,1,0).unit().dot(vec3(p1[0],0,p1[1])) < 0) {
			Delaunay::Point temp = p1;
			p1 = p2;
			p2 = temp;
		}

		output.push_back(tri_triangle(tri_vertex(p1[0],p1[1]),tri_vertex(p2[0],p2[1]),tri_vertex(p3[0],p3[1])));
	}

	return output;
}

enum editing {
	UNDEFINED,
	VERTEX,
	LINEDEF,
	SIDEDEF,
	SECTOR,
	THING
};

Map::Map(Game* game, std::string mapname) {
	m_game = game;
	std::string wad_path = m_game->GetWADPath();

	m_sectors = std::vector<Sector>(0);
	
	lumpdata mapdata = extract_lump_from_wad(wad_path, mapname, "", false, 1);
	if (!mapdata.successful) return;

	std::vector<udmf_vertex> vertices;
	std::vector<udmf_linedef> linedefs;
	std::vector<udmf_sidedef> sidedefs;
	std::vector<udmf_sector> sectors;
	std::vector<udmf_thing> things;

	udmf_vertex c_vertex;
	udmf_linedef c_linedef;
	udmf_sidedef c_sidedef;
	udmf_sector c_sector;
	udmf_thing c_thing;

	editing state = UNDEFINED;

	const char* data_char = (const char*)mapdata.data;
	std::string data_str(data_char);
	std::vector<std::string> data = split_string(data_str, "\n");
	for (auto i : data) {
		i = trim_trailing_comment(i);
		i = trim_whitespace(i);
		std::vector<std::string> split = split_string(i, "=");
		for (auto& i : split) {
			i = trim_whitespace(i);
			i = trim_trailing_comment(i);
			i = trim_trailing_comment(i, ";");
			i = trim_whitespace(i, "\"");
		}
		std::string tag = split.size() > 0 ? split[0] : "";
		std::string value = split.size() > 1 ? split[1] : "";
		for (size_t i = 2; i < split.size(); i++)
			value += "=" + split[i];
		switch (state) {
			case UNDEFINED: {
				if (tag == "vertex") state = VERTEX;
				else if (tag == "linedef") state = LINEDEF;
				else if (tag == "sidedef") state = SIDEDEF;
				else if (tag == "sector") state = SECTOR;
				else if (tag == "thing") state = THING;
				break;
			}
			case VERTEX: {
				if (tag == "x") c_vertex.x=atof(value.c_str());
				else if (tag == "y") c_vertex.y=-atof(value.c_str());
				else if (tag == "}") {
					state = UNDEFINED;
					vertices.push_back(c_vertex);
					c_vertex = udmf_vertex{};
				}
				break;
			}
			case LINEDEF: {
				if (tag == "v1") c_linedef.v1=atoi(value.c_str());
				else if (tag == "v2") c_linedef.v2=atoi(value.c_str());
				else if (tag == "sidefront") c_linedef.sidefront=atoi(value.c_str());
				else if (tag == "sideback") c_linedef.sideback=atoi(value.c_str());
				else if (tag == "twosided") c_linedef.twosided=(value=="true");
				else if (tag == "}") {
					state = UNDEFINED;
					linedefs.push_back(c_linedef);
					c_linedef = udmf_linedef{};
				}
				break;
			}
			case SIDEDEF: {
				if (tag == "offsetx_top") c_sidedef.offsetx_top=atoi(value.c_str());
				else if (tag == "offsety_top") c_sidedef.offsety_top=atoi(value.c_str());
				else if (tag == "offsetx_bottom") c_sidedef.offsetx_bottom=atoi(value.c_str());
				else if (tag == "offsety_bottom") c_sidedef.offsety_bottom=atoi(value.c_str());
				else if (tag == "offsetx_mid") c_sidedef.offsetx_mid=atoi(value.c_str());
				else if (tag == "offsety_mid") c_sidedef.offsety_mid=atoi(value.c_str());
				else if (tag == "texturebottom") c_sidedef.texturebottom=value;
				else if (tag == "texturetop") c_sidedef.texturetop=value;
				else if (tag == "texturemiddle") c_sidedef.texturemiddle=value;
				else if (tag == "sector") c_sidedef.sector=atoi(value.c_str());
				else if (tag == "}") {
					state = UNDEFINED;
					sidedefs.push_back(c_sidedef);
					c_sidedef = udmf_sidedef{};
				}
				break;
			}
			case SECTOR: {
				if (tag == "heightfloor") c_sector.heightfloor=atoi(value.c_str());
				else if (tag == "heightceiling") c_sector.heightceiling=atoi(value.c_str());
				else if (tag == "texturefloor") c_sector.texturefloor=value;
				else if (tag == "textureceiling") c_sector.textureceiling=value;
				else if (tag == "lightlevel") c_sector.lightlevel=atoi(value.c_str());
				else if (tag == "}") {
					state = UNDEFINED;
					sectors.push_back(c_sector);
					c_sector = udmf_sector{};
				}
				break;
			}
			case THING: {
				if (tag == "x") c_thing.x=atof(value.c_str());
				else if (tag == "y") c_thing.y=atof(value.c_str());
				else if (tag == "height") c_thing.height=atof(value.c_str());
				else if (tag == "angle") c_thing.angle=atoi(value.c_str());
				else if (tag == "comment") {
					c_thing.data=value;
				}
				else if (tag == "}") {
					state = UNDEFINED;
					things.push_back(c_thing);
					c_thing = udmf_thing{};
				}
				break;
			}
		}
	}
	
	m_map_segments = std::vector<MapSegmentRenderData>();
	m_lines = std::vector<line>();

	m_sectors = std::vector<Sector>();

	for (size_t i = 0; i < sectors.size(); i++) {
		udmf_sector current_sector = sectors[i];
		std::vector<udmf_sidedef> connected_sidedefs;
		std::vector<size_t> connected_sidedef_ids;
		for (size_t j = 0; j < sidedefs.size(); j++) {
			udmf_sidedef potential_sidedef = sidedefs[j];
			if (potential_sidedef.sector == i) {
				connected_sidedefs.push_back(potential_sidedef);
				connected_sidedef_ids.push_back(j);
			}
		}
		std::vector<udmf_linedef> connected_linedefs;
		for (size_t j = 0; j < connected_sidedefs.size(); j++) {
			size_t id = connected_sidedef_ids[j];
			for (auto potential_linedef : linedefs) {
				if (potential_linedef.sidefront == id || potential_linedef.sideback == id)
					connected_linedefs.push_back(potential_linedef);
			}
		}

		std::vector<tri_edge> floor_edges;

		int realfloor = current_sector.heightfloor;
		int realceiling = current_sector.heightceiling;

		for (auto current_linedef : connected_linedefs) {
			udmf_vertex v1 = vertices[current_linedef.v1];
			udmf_vertex v2 = vertices[current_linedef.v2];
			tri_edge new_edge(tri_vertex(v1.x, v1.y), tri_vertex(v2.x, v2.y));
			bool add = true; for (auto i : floor_edges) if (i == new_edge) add = false;
			if (add) floor_edges.push_back(tri_edge(tri_vertex(v1.x,v1.y),tri_vertex(v2.x,v2.y)));

			udmf_sidedef current_sidedef;
			udmf_sidedef alt_sidedef;
			udmf_sector alt_sector;
			int midfloor;
			int midceiling;
			if (current_linedef.sideback != -1) {
				if (sidedefs[current_linedef.sidefront].sector == i) {current_sidedef = sidedefs[current_linedef.sidefront]; alt_sidedef = sidedefs[current_linedef.sideback];}
				else {current_sidedef = sidedefs[current_linedef.sideback]; alt_sidedef = sidedefs[current_linedef.sidefront];}
				alt_sector = sectors[alt_sidedef.sector];
				
				if (current_sector.heightfloor >= alt_sector.heightfloor) midfloor = current_sector.heightfloor;
				else midfloor = alt_sector.heightfloor;
				
				if (current_sector.heightceiling <= alt_sector.heightceiling) midceiling = current_sector.heightceiling;
				else midceiling = alt_sector.heightceiling;
			} else {
				current_sidedef = sidedefs[current_linedef.sidefront];
				midfloor = current_sector.heightfloor;
				midceiling = current_sector.heightceiling;
			}
			float length = sqrt((v2.x-v1.x)*(v2.x-v1.x) + (v2.y-v1.y)*(v2.y-v1.y));

			float norm_x = -(v2.y-v1.y)/length;
			float norm_y =  (v2.x-v1.x)/length;

			if (!current_sidedef.texturemiddle.empty()) {
				std::shared_ptr<Material> material = m_game->GetResourceManager()->GetMaterial(current_sidedef.texturemiddle);
				line l = line{vec2{v1.x,v1.y}/SCALE, vec2{v2.x,v2.y}/SCALE};
				vec2 ln = l.n();
				if (ln.dot(vec2{norm_x, norm_y}) < 0) {
					vec2 tmp = l.a;
					l.a = l.b;
					l.b = tmp;
				}
				m_lines.push_back(l);

				float v1u = current_sidedef.offsetx_mid/SCALE;
				float v2u = length/SCALE + current_sidedef.offsetx_mid/SCALE;
				float lowv = (midfloor+current_sidedef.offsety_mid)/SCALE;
				float highv = (midceiling+current_sidedef.offsety_mid)/SCALE;
				
				mesh_vertex vertex_data[6] = {
					{glm::vec3{v1.x/SCALE,midfloor/SCALE,v1.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v1u,lowv}},
					{glm::vec3{v2.x/SCALE,midfloor/SCALE,v2.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v2u,lowv}},
					{glm::vec3{v1.x/SCALE,midceiling/SCALE,v1.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v1u,highv}},
					{glm::vec3{v1.x/SCALE,midceiling/SCALE,v1.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v1u,highv}},
					{glm::vec3{v2.x/SCALE,midfloor/SCALE,v2.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v2u,lowv}},
					{glm::vec3{v2.x/SCALE,midceiling/SCALE,v2.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v2u,highv}},
				};
				GLuint index_data[6] = {
					0,1,2,
					4,5,6
				};
				
				addmapsegment(m_map_segments, vertex_data, index_data, 6, material);
			}
			if (!current_sidedef.texturebottom.empty() && midfloor > realfloor) {
				std::shared_ptr<Material> material = m_game->GetResourceManager()->GetMaterial(current_sidedef.texturebottom);

				float v1u = current_sidedef.offsetx_bottom/SCALE;
				float v2u = length/SCALE + current_sidedef.offsetx_bottom/SCALE;
				float lowv = (realfloor+current_sidedef.offsety_bottom)/SCALE;
				float highv = (midfloor+current_sidedef.offsety_bottom)/SCALE;

				mesh_vertex vertex_data[6] = {
					{glm::vec3{v1.x/SCALE,realfloor/SCALE,v1.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v1u,lowv}},
					{glm::vec3{v2.x/SCALE,realfloor/SCALE,v2.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v2u,lowv}},
					{glm::vec3{v1.x/SCALE,midfloor/SCALE,v1.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v1u,highv}},
					{glm::vec3{v1.x/SCALE,midfloor/SCALE,v1.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v1u,highv}},
					{glm::vec3{v2.x/SCALE,realfloor/SCALE,v2.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v2u,lowv}},
					{glm::vec3{v2.x/SCALE,midfloor/SCALE,v2.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v2u,highv}},
				};
				GLuint index_data[6] = {
					0,1,2,
					3,4,5
				};
				addmapsegment(m_map_segments, vertex_data, index_data, 6, material);
			}
			if (!current_sidedef.texturetop.empty() && midceiling < realceiling) {
				std::shared_ptr<Material> material = m_game->GetResourceManager()->GetMaterial(current_sidedef.texturetop);

				float v1u = current_sidedef.offsetx_top/SCALE;
				float v2u = length/SCALE + current_sidedef.offsetx_top/SCALE;
				
				float lowv = (midceiling+current_sidedef.offsety_top)/SCALE;
				float highv = (realceiling+current_sidedef.offsety_top)/SCALE;

				mesh_vertex vertex_data[6] = {
					{glm::vec3{v1.x/SCALE,midceiling/SCALE,v1.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v1u,lowv}},
					{glm::vec3{v2.x/SCALE,midceiling/SCALE,v2.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v2u,lowv}},
					{glm::vec3{v1.x/SCALE,realceiling/SCALE,v1.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v1u,highv}},
					{glm::vec3{v1.x/SCALE,realceiling/SCALE,v1.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v1u,highv}},
					{glm::vec3{v2.x/SCALE,midceiling/SCALE,v2.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v2u,lowv}},
					{glm::vec3{v2.x/SCALE,realceiling/SCALE,v2.y/SCALE},glm::vec3{norm_x,0.0f,norm_y},glm::vec2{v2u,highv}},
				};
				GLuint index_data[6] = {
					0,1,2,
					4,5,6
				};
				addmapsegment(m_map_segments, vertex_data, index_data, 6, material);
			}
		}
		
		std::vector<tri_triangle> floor_triangles = edges_to_faces(floor_edges);
		std::vector<triangle> phys_triangles(floor_triangles.size());
		for (size_t i = 0; i < floor_triangles.size(); i++)
			phys_triangles[i] = triangle{
				vec2{floor_triangles[i].v0.x/SCALE, floor_triangles[i].v0.y/SCALE},
				vec2{floor_triangles[i].v1.x/SCALE, floor_triangles[i].v1.y/SCALE},
				vec2{floor_triangles[i].v2.x/SCALE, floor_triangles[i].v2.y/SCALE}
			};

		m_sectors.push_back(Sector{
			current_sector.heightfloor/SCALE,
			current_sector.heightceiling/SCALE,
			phys_triangles
		});

		mesh_vertex* floor_vertex_data = new mesh_vertex[floor_triangles.size()*3];
		mesh_vertex* ceiling_vertex_data = new mesh_vertex[floor_triangles.size()*3];
		GLuint* index_data = new GLuint[floor_triangles.size()*3];

		std::shared_ptr<Material> floor_material = m_game->GetResourceManager()->GetMaterial(current_sector.texturefloor);
		std::shared_ptr<Material> ceiling_material = m_game->GetResourceManager()->GetMaterial(current_sector.textureceiling);

		for (size_t j = 0; j < floor_triangles.size(); j++) {
			size_t index = j*3;
			tri_triangle triangle = floor_triangles[j];
			floor_vertex_data[index] = mesh_vertex{glm::vec3{triangle.v0.x/SCALE, realfloor/SCALE, triangle.v0.y/SCALE}, glm::vec3{0,1,0}, glm::vec2{triangle.v0.x/SCALE,triangle.v0.y/SCALE}};
			floor_vertex_data[index+1] = mesh_vertex{glm::vec3{triangle.v2.x/SCALE, realfloor/SCALE, triangle.v2.y/SCALE}, glm::vec3{0,1,0}, glm::vec2{triangle.v2.x/SCALE,triangle.v2.y/SCALE}};
			floor_vertex_data[index+2] = mesh_vertex{glm::vec3{triangle.v1.x/SCALE, realfloor/SCALE, triangle.v1.y/SCALE}, glm::vec3{0,1,0}, glm::vec2{triangle.v1.x/SCALE,triangle.v1.y/SCALE}};
			ceiling_vertex_data[index] = mesh_vertex{glm::vec3{triangle.v0.x/SCALE, realceiling/SCALE, triangle.v0.y/SCALE}, glm::vec3{0,-1,0}, glm::vec2{triangle.v0.x/SCALE,triangle.v0.y/SCALE}};
			ceiling_vertex_data[index+1] = mesh_vertex{glm::vec3{triangle.v1.x/SCALE, realceiling/SCALE, triangle.v1.y/SCALE}, glm::vec3{0,-1,0}, glm::vec2{triangle.v1.x/SCALE,triangle.v1.y/SCALE}};
			ceiling_vertex_data[index+2] = mesh_vertex{glm::vec3{triangle.v2.x/SCALE, realceiling/SCALE, triangle.v2.y/SCALE}, glm::vec3{0,-1,0}, glm::vec2{triangle.v2.x/SCALE,triangle.v2.y/SCALE}};
			index_data[index] = index;
			index_data[index+1] = index+1;
			index_data[index+2] = index+2;
		}
		
		addmapsegment(m_map_segments, floor_vertex_data, index_data, floor_triangles.size()*3, floor_material);
		addmapsegment(m_map_segments, ceiling_vertex_data, index_data, floor_triangles.size()*3, floor_material);
	}
	
	for (udmf_thing thing : things) {
		std::string name;
		std::string scriptname;
		std::string meshname;
		std::string matname;
		std::string bbvalue;
		thing.data = trim_double(thing.data);
		std::vector<std::string> values = split_string(thing.data, " ");
		for (std::string i : values) {
			i = trim_whitespace(i);
			std::vector<std::string> split = split_string(i, "=");
			std::string thing_tag = split[0];
			std::string thing_value = split[1];
			if (thing_tag == "name") name = thing_value;
			else if (thing_tag == "script") scriptname = trim_whitespace(thing_value);
			else if (thing_tag == "mesh") meshname = trim_whitespace(thing_value);
			else if (thing_tag == "mat") matname = trim_whitespace(thing_value);
			else if (thing_tag == "bb") bbvalue = trim_whitespace(thing_value);
		}
		if (name.empty()) {
			printf("Error: all game objects in the map must at least have a name.\n");
			printf("Errored thing: %s\n", thing.data.c_str());
			continue;
		}
		std::shared_ptr<Mesh> mesh = nullptr;
		std::shared_ptr<Material> mat = nullptr;
		if (!meshname.empty() && meshname != "NULL") mesh = m_game->GetResourceManager()->GetMesh(meshname);
		if (!matname.empty() && matname != "NULL") mat = m_game->GetResourceManager()->GetMaterial(matname);
		float bbx;
		float bby;
		if (!bbvalue.empty()) {
			std::vector<std::string> bbsplit = split_string(bbvalue, ",");
			bbx = atof(bbsplit[0].c_str());
			bby = atof(bbsplit[1].c_str());
		}
		m_game->GetWorkspace()->CreateGameObject(
			name, nullptr, scriptname,
			mesh, mat,
			Transform{vec3{thing.x/SCALE, thing.height/SCALE, thing.y/SCALE}, vec3{0,M_PI_2+thing.angle/180.0*M_PI,0}, vec3{1,1,1}},
			Box{vec2{bbx,bby},vec2{thing.x/SCALE,thing.y/SCALE}}
		);
	}
}

Map::~Map() {
	m_game = nullptr;

	for (auto map_segment : m_map_segments) {
		glDeleteVertexArrays(1, &map_segment.vao);
		glDeleteBuffers(1, &map_segment.vbo);
		glDeleteBuffers(1, &map_segment.ibo);
	}
	m_map_segments.clear();
}

std::vector<MapSegmentRenderData> Map::GetMapSegments() {
	return m_map_segments;
}

std::vector<line> Map::GetLines() {
	return m_lines;
}

std::optional<Sector> Map::GetHighestOverlappingSector(Box& box) {
	if (m_sectors.size() == 0) return std::optional<Sector>();
	Sector highest = m_sectors[0];
	for (Sector sector : m_sectors) {
		if (sector.heightfloor < highest.heightfloor) continue;
		bool overlapping = false;
		for (triangle t : sector.triangles) {
			if (overlap_box_triangle(box, t)) {overlapping = true; break;}
		}
		if (overlapping) highest = sector;
	}
	return highest;
}
