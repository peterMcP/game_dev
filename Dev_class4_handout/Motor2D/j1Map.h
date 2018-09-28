#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

//#define MAX_TILESETS 10

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------
struct layerInfo
{
	p2SString name;
	uint width = 0;
	uint height = 0;
};

struct TileSetInfo
{
	uint firstgid = 0;
	p2SString name;// = "defaultName";
	uint tilewidth = 0;
	uint tileheight = 0;
	int spacing = -1;
	int margin = -1;
	uint tilecount = 0;
	uint columns = 0;
	SDL_Texture* image = nullptr;
};


// TODO 1: Create a struct needed to hold the information to Map node

enum class renderOrder
{
	invalidRender = -1,
	right_down,
	right_up,
	left_down,
	left_up
};

enum class orientation
{
	orthogonal,
	isometric
};

struct dataMap
{
	float version = 0;
	renderOrder renderOrder = renderOrder::invalidRender;
	orientation orientation = orientation::orthogonal;
	uint width = 0;
	uint height = 0; 
	uint tilewidth = 0;
	uint tileheight = 0;
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

	bool loadMapData(pugi::xml_node&);
	bool loadTilesetData(pugi::xml_node&);
	bool loadMapLayers(pugi::xml_node&);

public:

	// TODO 1: Add your struct for map info as public for now
	dataMap mapInfo;
	//TileSetInfo tilesetInfo[MAX_TILESETS];
	p2List<TileSetInfo*> tilesets;
	TileSetInfo* tileset;
	uint tilesetCounter = 0;
	//HOMEWORK
	p2List<layerInfo*> layerList;
	layerInfo* layer;
	uint layerCounter = 0;


private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__