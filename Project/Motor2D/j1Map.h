#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------

struct tileSet
{
	uint firstgid = 0u;
	char* name;
	uint tilewidth = 0u;
	uint tileheight = 0u;
	uint spacing = 0u;
	uint margin = 0u;

	struct image
	{
		uint width = 0u;
		uint height = 0u;
		char* source;
	};
};

// TODO 1: Create a struct needed to hold the information to Map node


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

	bool LoadMap(pugi::xml_document& config_file);

public:

	// TODO 1: Add your struct for map info as public for now

	struct data
	{
		float version = 1.0f;
		enum class orientation
		{
			INVALID,
			ORTHOGONAL,
			ISOMETRIC,
			STAGGERED,
			HEXAGONAL
		};
		enum renderorder
		{
			RO_INVALID,
			RO_RIGHTDOWN,
			RO_RIGHTUP,
			RO_LEFTDOWN,
			RO_LEFTUP
		};

		orientation orientation = orientation::INVALID;
		renderorder renderorder = RO_INVALID;
		uint width = 0u;
		uint height = 0u;
		uint tilewidth = 0u;
		uint tileheight = 0u;
		uint nextobjectid = 0u;
	};


private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	data                map_data;
};

#endif // __j1MAP_H__