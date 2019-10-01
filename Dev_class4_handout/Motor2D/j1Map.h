#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------
struct Tileset
{
	uint firstgid = 0u;
	char* name = NULL;
	uint tile_width = 0u;
	uint tile_height = 0u;
	uint spacing = 0u;
	uint margin = 0u;
	char* image_source = NULL;
};
// TODO 1: Create a struct needed to hold the information to Map node
//options for orientation
enum orientation 
{
	or_orthogonal,
	or_isometric,
	or_staggered,
	or_hexagonal,
	or_unknown
};
//options for render order
enum render_order 
{
	ro_right_down,
	ro_right_up,
	ro_left_down,
	ro_left_up,
	ro_uknown
};

struct Map
{
	enum orientation orientation::or_unknown;
	enum render_order render_order::ro_uknown;
	uint width = 0u;
	uint height = 0u;
	uint tile_width = 0u;
	uint tile_height = 0u;
	uint next_object_id = 0u;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

private:


public:

	// TODO 1: Add your struct for map info as public for now
	Map map_1;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__