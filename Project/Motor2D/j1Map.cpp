#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map


	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data

		LoadMap(map_file);
		map_loaded = true;
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	

	if(ret == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
	}

	map_loaded = ret;

	return ret;
}

 bool j1Map::LoadMap(pugi::xml_document& map_file)
{
	pugi::xml_node ret;
	
	//pugi::xml_parse_result result = map_file.load_file(file_name);

	ret = map_file.child("map");

	if (ret.attribute("orientation").as_string == "orthogonal")
		map_data.orientation = data::orientation::ORTHOGONAL;
	else if (ret.attribute("orientation").as_string == "isometric")
		map_data.orientation = data::orientation::ISOMETRIC;
	else if (ret.attribute("orientation").as_string == "staggered")
		map_data.orientation = data::orientation::STAGGERED;
	else if (ret.attribute("orientation").as_string == "hexagonal")
		map_data.orientation = data::orientation::HEXAGONAL;
	else
		map_data.orientation = data::orientation::INVALID;

	if (ret.attribute("renderorder").as_string == "right-down")
		map_data.renderorder = data::RO_RIGHTDOWN;
	else if (ret.attribute("renderorder").as_string == "right-up")
		map_data.renderorder = data::RO_RIGHTUP;
	else if (ret.attribute("renderorder").as_string == "left-down")
		map_data.renderorder = data::RO_LEFTDOWN;
	else if (ret.attribute("renderorder").as_string == "left-up")
		map_data.renderorder = data::RO_LEFTUP;
	else
		map_data.renderorder = data::RO_INVALID;

	map_data.width = ret.attribute("width").as_uint;
	map_data.height = ret.attribute("height").as_uint;
	map_data.tilewidth = ret.attribute("tilewidth").as_uint;
	map_data.tileheight = ret.attribute("tileheight").as_uint;
	map_data.nextobjectid = ret.attribute("nextobjectid").as_uint;

	if (map_data.orientation != data::orientation::INVALID)
		return true;
	else
	{
		LOG("MAP INFORMATION COULDNT BE TRANSFERED");
		return false;
	}
}
